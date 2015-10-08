/*
 * action.c
 *
 *  Created on: 17.10.2012
 *      Author: Stefan Krupop
 */

#include "action.h"
#include "dbgu/dbgu.h"
#include "eeprom/eeprom.h"
#include "servo/servo.h"
#include "tc/tc.h"
#include <string.h>
#include <math.h>


#define ACTION_PROCESS_INTERVAL	8 // ms

#define MOTION_ANGLE_TO_VAL1024(angle)	(lroundf(-(angle) * 3.413) + 512) // 1024 / 300 = 3.413
#define MOTION_VAL1024_TO_ANGLE(value)	(-((value) - 512) * 0.293) // 300 / 1024 = 0.293


unsigned char m_Playing;
PAGE m_PlayPage;
PAGE m_NextPlayPage;
int m_IndexPlayingPage;
unsigned char m_FirstDrivingStart;
unsigned char m_StopPlaying;
int m_PageStepCount;
unsigned char m_PlayingFinished;

// Prototypes
void Action_Process(void);

void Action_Init() {
	m_Playing = 0;
}

void Action_SetChecksum(PAGE *pPage) {
	unsigned char checksum = 0x00;
	unsigned char *pt = (unsigned char*) pPage;

	pPage->header.checksum = 0x00;

	for (unsigned int i = 0; i < sizeof(PAGE); i++) {
		checksum += *pt;
		pt++;
	}

	pPage->header.checksum = (unsigned char) (0xff - checksum);
}

void Action_ResetPage(PAGE *pPage) {
	unsigned char *pt = (unsigned char*) pPage;

	for (unsigned int i = 0; i < sizeof(PAGE); i++) {
		*pt = 0x00;
		pt++;
	}

	pPage->header.schedule = TIME_BASE_SCHEDULE; // default time base
	pPage->header.repeat = 1;
	pPage->header.speed = 32;
	pPage->header.accel = 32;

	for (int i = 0; i < NUMBER_OF_JOINTS; i++)
		pPage->header.slope[i] = 0x55;

	for (int i = 0; i < MAXNUM_STEP; i++) {
		for (int j = 0; j < 31; j++)
			pPage->step[i].position[j] = INVALID_BIT_MASK;

		pPage->step[i].pause = 0;
		pPage->step[i].time = 0;
	}

	Action_SetChecksum(pPage);
}

unsigned char Action_VerifyChecksum(PAGE *pPage) {
	unsigned char checksum = 0x00;
	unsigned char *pt = (unsigned char*) pPage;

	for (unsigned int i = 0; i < sizeof(PAGE); ++i) {
		checksum += *pt;
		pt++;
	}

	if (checksum != 0xff) {
		dbgu_printf("Checksum error\r\n");
		return 0;
	}

	return 1;
}

unsigned char Action_Start(int iPage) {
	if (iPage < 1 || iPage >= MAXNUM_PAGE) {
		dbgu_printf("Can not play page. (%d is invalid index)\n", iPage);
		return 0;
	}

	PAGE page;
	if (Action_LoadPage(iPage, &page) == 0)
		return 0;

	return Action_Start_Page(iPage, &page);
}

unsigned char Action_Start_Name(char* namePage) {
	int idx;
	PAGE page;

	for (idx = 1; idx < MAXNUM_PAGE; idx++) {
		if (Action_LoadPage(idx, &page) == 0)
			return 0;

		if (strcmp(namePage, (char*) page.header.name) == 0)
			break;
	}

	return Action_Start_Page(idx, &page);
}

unsigned char Action_Start_Page(int idx, PAGE *pPage) {
	if (m_Playing == 1) {
		dbgu_printf("Can not play page %d.(Now playing)\n", idx);
		return 0;
	}

	m_PlayPage = *pPage;

	if (m_PlayPage.header.repeat == 0 || m_PlayPage.header.stepnum == 0) {
		dbgu_printf("Page %d has no action\n", idx);
		return 0;
	}

	m_IndexPlayingPage = idx;
	m_FirstDrivingStart = 1;
	m_Playing = 1;

	TC_SetTimer1Interval(ACTION_PROCESS_INTERVAL);
	TC_AssignTimer1Callback(Action_Process);

	return 1;
}

void Action_Stop() {
	m_StopPlaying = 1;
}

void Action_Brake() {
	m_Playing = 0;
}

unsigned char Action_IsRunning() {
	return m_Playing;
}

/*unsigned char Action_IsRunning_Page(int *iPage, int *iStep) {
	if (iPage != 0)
		*iPage = m_IndexPlayingPage;

	if (iStep != 0)
		*iStep = m_PageStepCount - 1;

	return Action_IsRunning();
}*/

unsigned char Action_LoadPage(int idx, PAGE *pPage) {
	unsigned int position = (unsigned int) (sizeof(PAGE) * idx);
	unsigned char *pt = (unsigned char*) pPage;

	eeprom_readRaw(EEPROM_MOTION_ID, position, pt, sizeof(PAGE));

/*	dbgu_printf("Page dump:\r\n");
	for (int i = 0; i < sizeof(PAGE); ++i) {
		dbgu_print_char_in_hex(pt[i]);
		if (i % 16 == 15) {
			dbgu_print_ascii("\r\n");
		} else {
			dbgu_print_char(' ');
		}
	} */

	if (Action_VerifyChecksum(pPage) == 0)
		Action_ResetPage(pPage);

	return 1;
}

unsigned char Action_SavePage(int idx, PAGE *pPage) {
	unsigned int position = (unsigned int) (sizeof(PAGE) * idx);
	unsigned char *pt = (unsigned char*) pPage;

	if(Action_VerifyChecksum(pPage) == 0)
        Action_SetChecksum(pPage);

    eeprom_writeRaw(EEPROM_MOTION_ID, position, pt, sizeof(PAGE));

	return 1;
}

void Action_Process() {
    unsigned char bID;

    ///////////////// Static
	static unsigned short wpStartAngle1024[NUMBER_OF_JOINTS]; // 보간할 시작 지점
    static unsigned short wpTargetAngle1024[NUMBER_OF_JOINTS]; // 보간할 도착 지점
    static short int ipMovingAngle1024[NUMBER_OF_JOINTS]; // 총 가야할 거리
    static short int ipMainAngle1024[NUMBER_OF_JOINTS]; // 등속 구간에서 가야할 거리
    static short int ipAccelAngle1024[NUMBER_OF_JOINTS]; // 가속 구간에서 가야할 거리
    static short int ipMainSpeed1024[NUMBER_OF_JOINTS]; // 목표 등속도
    static short int ipLastOutSpeed1024[NUMBER_OF_JOINTS]; // 이 전 상태의 속도(관성)
    static short int ipGoalSpeed1024[NUMBER_OF_JOINTS]; // 모터가 내야 할 목표속도
    static unsigned char bpFinishType[NUMBER_OF_JOINTS]; // 도착 지점에 도달할 상태
    static unsigned short wUnitTimeCount;
    static unsigned short wUnitTimeNum;
    static unsigned short wPauseTime;
    static unsigned short wUnitTimeTotalNum;
    static unsigned short wAccelStep;
    static unsigned char bSection;
    static unsigned char bPlayRepeatCount;
    static unsigned short wNextPlayPage;

    /////////////// Enum 변수

    /**************************************
    * Section             /----\
    *                    /|    |\
    *        /+---------/ |    | \
    *       / |        |  |    |  \
    * -----/  |        |  |    |   \----
    *      PRE  MAIN   PRE MAIN POST PAUSE
    ***************************************/
    enum{ PRE_SECTION, MAIN_SECTION, POST_SECTION, PAUSE_SECTION };
    enum{ ZERO_FINISH, NONE_ZERO_FINISH};

    if( m_Playing == 0 ) {
    	TC_AssignTimer1Callback(0); // Disable timer callback
        return;
    }

    if( m_FirstDrivingStart == 1 ) // 처음 시작할때
    {
        m_FirstDrivingStart = 0; //First Process end
        m_PlayingFinished = 0;
		m_StopPlaying = 0;
        wUnitTimeCount = 0;
        wUnitTimeNum = 0;
        wPauseTime = 0;
        bSection = PAUSE_SECTION;
        m_PageStepCount = 0;
        bPlayRepeatCount = m_PlayPage.header.repeat;
        wNextPlayPage = 0;

		for( bID=MIN_SERVO_ID; bID<NUMBER_OF_JOINTS; bID++ ) {
			wpTargetAngle1024[bID] = MOTION_ANGLE_TO_VAL1024(Servo_GetPosition(bID));
			ipLastOutSpeed1024[bID] = 0;
			ipMovingAngle1024[bID] = 0;
			ipGoalSpeed1024[bID] = 0;
		}
    }

    if( wUnitTimeCount < wUnitTimeNum ) // 현재 진행중이라면
    {
        wUnitTimeCount++;
        if( bSection == PAUSE_SECTION )
        {
        }
        else
        {
            for( bID=MIN_SERVO_ID; bID<NUMBER_OF_JOINTS; bID++ )
            {
					if( ipMovingAngle1024[bID] == 0 ) {
						Servo_SetPosition(bID, MOTION_VAL1024_TO_ANGLE(wpStartAngle1024[bID]));
					} else {
						if( bSection == PRE_SECTION )
						{
							short int iSpeedN = (short)(((long)(ipMainSpeed1024[bID] - ipLastOutSpeed1024[bID]) * wUnitTimeCount) / wUnitTimeNum);
							ipGoalSpeed1024[bID] = ipLastOutSpeed1024[bID] + iSpeedN;
							ipAccelAngle1024[bID] =  (short)((((long)(ipLastOutSpeed1024[bID] + (iSpeedN >> 1)) * wUnitTimeCount * 144) / 15) >> 9);

							Servo_SetPosition(bID, MOTION_VAL1024_TO_ANGLE(wpStartAngle1024[bID] + ipAccelAngle1024[bID]));
						}
						else if( bSection == MAIN_SECTION )
						{
							Servo_SetPosition(bID, MOTION_VAL1024_TO_ANGLE(wpStartAngle1024[bID] + (short int)(((long)(ipMainAngle1024[bID])*wUnitTimeCount) / wUnitTimeNum)));
							ipGoalSpeed1024[bID] = ipMainSpeed1024[bID];
						}
						else // POST_SECTION
						{
							if( wUnitTimeCount == (wUnitTimeNum-1) )
							{
								// 스텝 마지막 오차를 줄이기위해 그냥 목표 위치 값을 사용
								Servo_SetPosition(bID, MOTION_VAL1024_TO_ANGLE(wpTargetAngle1024[bID]));
							}
							else
							{
								if( bpFinishType[bID] == ZERO_FINISH )
								{
									short int iSpeedN = (short int)(((long)(0 - ipLastOutSpeed1024[bID]) * wUnitTimeCount) / wUnitTimeNum);
									ipGoalSpeed1024[bID] = ipLastOutSpeed1024[bID] + iSpeedN;
									Servo_SetPosition(bID, MOTION_VAL1024_TO_ANGLE(wpStartAngle1024[bID] +  (short)((((long)(ipLastOutSpeed1024[bID] + (iSpeedN>>1)) * wUnitTimeCount * 144) / 15) >> 9)));
								}
								else // NONE_ZERO_FINISH
								{
									// MAIN Section과 동일하게 작동-동일
									// step에서 어떤서보는 가고 어떤 서보는 서야하는 상황이 발생할 수 있으므로 이렇게 할 수밖에 없음
									Servo_SetPosition(bID, MOTION_VAL1024_TO_ANGLE(wpStartAngle1024[bID] + (short int)(((long)(ipMainAngle1024[bID]) * wUnitTimeCount) / wUnitTimeNum)));
									ipGoalSpeed1024[bID] = ipMainSpeed1024[bID];
								}
							}
						}
					}

					// TODO: Implement in servo "driver"
					//m_Joint.SetSlope(bID, 1 << (m_PlayPage.header.slope[bID]>>4), 1 << (m_PlayPage.header.slope[bID]&0x0f));
                    //m_Joint.SetPGain(bID, (256 >> (m_PlayPage.header.slope[bID]>>4)) << 2);
            }
            Servo_SendPositions();
        }
    }
    else if( wUnitTimeCount >= wUnitTimeNum ) // 현재 Section이 완료되었다면
    {
        wUnitTimeCount = 0;

        for( bID=MIN_SERVO_ID; bID<NUMBER_OF_JOINTS; bID++ )
        {
			wpStartAngle1024[bID] = MOTION_ANGLE_TO_VAL1024(Servo_GetPosition(bID));
			ipLastOutSpeed1024[bID] = ipGoalSpeed1024[bID];
        }

        // Section 업데이트 ( PRE -> MAIN -> POST -> (PAUSE or PRE) ... )
        if( bSection == PRE_SECTION )
        {
            // MAIN Section 준비
            bSection = MAIN_SECTION;
            wUnitTimeNum =  wUnitTimeTotalNum - (wAccelStep << 1);

            for( bID=MIN_SERVO_ID; bID<NUMBER_OF_JOINTS; bID++ )
            {
					if( bpFinishType[bID] == NONE_ZERO_FINISH )
					{
						if( (wUnitTimeTotalNum - wAccelStep) == 0 ) // 등속 구간이 전혀 없다면
							ipMainAngle1024[bID] = 0;
						else
							ipMainAngle1024[bID] = (short)((((long)(ipMovingAngle1024[bID] - ipAccelAngle1024[bID])) * wUnitTimeNum) / (wUnitTimeTotalNum - wAccelStep));
					}
					else // ZERO_FINISH
						ipMainAngle1024[bID] = ipMovingAngle1024[bID] - ipAccelAngle1024[bID] - (short int)((((long)ipMainSpeed1024[bID] * wAccelStep * 12) / 5) >> 8);
            }
        }
        else if( bSection == MAIN_SECTION )
        {
            // POST Section 준비
            bSection = POST_SECTION;
            wUnitTimeNum = wAccelStep;

            for( bID=MIN_SERVO_ID; bID<NUMBER_OF_JOINTS; bID++ )
			{
				ipMainAngle1024[bID] = ipMovingAngle1024[bID] - ipMainAngle1024[bID] - ipAccelAngle1024[bID];
			}
        }
        else if( bSection == POST_SECTION )
        {
            // Pause time 유무에따라 달라짐
            if( wPauseTime )
            {
                bSection = PAUSE_SECTION;
                wUnitTimeNum = wPauseTime;
            }
            else
            {
                bSection = PRE_SECTION;
            }
        }
        else if( bSection == PAUSE_SECTION )
        {
            // PRE Section 준비
            bSection = PRE_SECTION;

            for( bID=MIN_SERVO_ID; bID<NUMBER_OF_JOINTS; bID++ )
			{
				ipLastOutSpeed1024[bID] = 0;
			}
        }

        // PRE Section시에 모든 준비를 한다.
        if( bSection == PRE_SECTION )
        {
            if( m_PlayingFinished == 1 ) // 모션이 끝났다면
            {
                m_Playing = 0;
                return;
            }

            m_PageStepCount++;

            if( m_PageStepCount > m_PlayPage.header.stepnum ) // 현재 페이지 재생이 끝났다면
            {
                // 다음 페이지 복사
                m_PlayPage = m_NextPlayPage;
                if( m_IndexPlayingPage != wNextPlayPage )
                    bPlayRepeatCount = m_PlayPage.header.repeat;
                m_PageStepCount = 1;
                m_IndexPlayingPage = wNextPlayPage;
            }

            if( m_PageStepCount == m_PlayPage.header.stepnum ) // 마지막 스텝이라면
            {
                // 다음 페이지 로딩
                if( m_StopPlaying == 1 ) // 모션 정지 명령이 있다면
                {
                    wNextPlayPage = m_PlayPage.header.exit; // 다음 페이지는 Exit 페이지로
                }
                else
                {
                    bPlayRepeatCount--;
                    if( bPlayRepeatCount > 0 ) // 반복 횟수가 남았다면
                        wNextPlayPage = m_IndexPlayingPage; // 다음 페이지는 현재 페이지로
                    else // 반복을 다했다면
                        wNextPlayPage = m_PlayPage.header.next; // 다음 페이지는 Next 페이지로
                }

                if( wNextPlayPage == 0 ) // 재생할 다음 페이지가 없다면 현재 스텝까지하고 종료
                    m_PlayingFinished = 1;
                else
                {
                    // 다음페이지 로딩(같으면 메모리 복사, 다르면 파일 읽기)
                    if( m_IndexPlayingPage != wNextPlayPage )
                        Action_LoadPage( wNextPlayPage, &m_NextPlayPage );
                    else
                        m_NextPlayPage = m_PlayPage;

                    // 재생할 정보가 없다면 현재 스텝까지하고 종료
                    if( m_NextPlayPage.header.repeat == 0 || m_NextPlayPage.header.stepnum == 0 )
                        m_PlayingFinished = 1;
                }
            }

            //////// Step 파라미터 계산
            wPauseTime = (((unsigned short)m_PlayPage.step[m_PageStepCount-1].pause) << 5) / m_PlayPage.header.speed;
            unsigned short wMaxSpeed256 = ((unsigned short)m_PlayPage.step[m_PageStepCount-1].time * (unsigned short)m_PlayPage.header.speed) >> 5;
            if( wMaxSpeed256 == 0 )
                wMaxSpeed256 = 1;
            unsigned short wMaxAngle1024 = 0;

            ////////// Joint별 파라미터 계산
            for( bID=MIN_SERVO_ID; bID<NUMBER_OF_JOINTS; bID++ )
			{
					// 이전, 현재, 미래를 바탕으로 궤적을 계산
					ipAccelAngle1024[bID] = 0;

					// Find current target angle
					unsigned short wCurrentTargetAngle; // Target position
					if( m_PlayPage.step[m_PageStepCount-1].position[bID] & INVALID_BIT_MASK )
						wCurrentTargetAngle = wpTargetAngle1024[bID];
					else
						wCurrentTargetAngle = m_PlayPage.step[m_PageStepCount-1].position[bID];

					// Update start, prev_target, curr_target
					wpStartAngle1024[bID] = wpTargetAngle1024[bID];
					unsigned short wPrevTargetAngle = wpTargetAngle1024[bID]; // Start position
					wpTargetAngle1024[bID] = wCurrentTargetAngle;

					// Find Moving offset
					ipMovingAngle1024[bID] = (int)(wpTargetAngle1024[bID] - wpStartAngle1024[bID]);

					// Find Next target angle
					unsigned short wNextTargetAngle; // Next target position
					if( m_PageStepCount == m_PlayPage.header.stepnum ) // 현재 스텝이 마지막이라면
					{
						if( m_PlayingFinished == 1 ) // 끝날 예정이라면
							wNextTargetAngle = wCurrentTargetAngle;
						else
						{
							if( m_NextPlayPage.step[0].position[bID] & INVALID_BIT_MASK )
								wNextTargetAngle = wCurrentTargetAngle;
							else
								wNextTargetAngle = m_NextPlayPage.step[0].position[bID];
						}
					}
					else
					{
						if( m_PlayPage.step[m_PageStepCount].position[bID] & INVALID_BIT_MASK )
							wNextTargetAngle = wCurrentTargetAngle;
						else
							wNextTargetAngle = m_PlayPage.step[m_PageStepCount].position[bID];
					}

					// Find direction change
					unsigned char bDirectionChanged;
					if( ((wPrevTargetAngle < wCurrentTargetAngle) && (wCurrentTargetAngle < wNextTargetAngle))
						|| ((wPrevTargetAngle > wCurrentTargetAngle) && (wCurrentTargetAngle > wNextTargetAngle)) )
					{
						// 계속 증가하거나 감소하고, 혹은 같다면(즉, 불연속 점이 없다면)
						bDirectionChanged = 0;
					}
					else
					{
						bDirectionChanged = 1;
					}

					// Find finish type
					if( bDirectionChanged || wPauseTime || m_PlayingFinished == 1 )
					{
						bpFinishType[bID] = ZERO_FINISH;
					}
					else
					{
						bpFinishType[bID] = NONE_ZERO_FINISH;
					}

					if( m_PlayPage.header.schedule == SPEED_BASE_SCHEDULE )
					{
						unsigned short wTmp;
						//MaxAngle1024 update
						if( ipMovingAngle1024[bID] < 0 )
							wTmp = -ipMovingAngle1024[bID];
						else
							wTmp = ipMovingAngle1024[bID];

						if( wTmp > wMaxAngle1024 )
							wMaxAngle1024 = wTmp;
					}
            }

            //시간을 계산해서 다시 7.8msec로 나누다(<<7)-그시간동안에 7.8msec가 몇개들어가는지 계산한 것
            //단위 변환뒤에 각/속도를 구하고(시간)그 시간에 다시 7.8msec가 몇개들어가있는지 계산
            //단위 변환 ---  각 :1024계->300도계,  속도: 256계 ->720계
            //wUnitTimeNum = ((wMaxAngle1024*300/1024) /(wMaxSpeed256 * 720/256)) /7.8msec;
            //             = ((128*wMaxAngle1024*300/1024) /(wMaxSpeed256 * 720/256)) ;    (/7.8msec == *128)
            //             = (wMaxAngle1024*40) /(wMaxSpeed256 *3);
            if( m_PlayPage.header.schedule == TIME_BASE_SCHEDULE )
                wUnitTimeTotalNum  = wMaxSpeed256; //TIME BASE 051025
            else
                wUnitTimeTotalNum  = (wMaxAngle1024 * 40) / (wMaxSpeed256 * 3);

            wAccelStep = m_PlayPage.header.accel;
            if( wUnitTimeTotalNum <= (wAccelStep << 1) )
            {
                if( wUnitTimeTotalNum == 0 )
                    wAccelStep = 0;
                else
                {
                    wAccelStep = (wUnitTimeTotalNum - 1) >> 1;
                    if( wAccelStep == 0 )
                        wUnitTimeTotalNum = 0; //움직이려면 적어도 가속,등속이 한 스텝이상씩은 존재해야
                }
            }

            unsigned long ulTotalTime256T = ((unsigned long)wUnitTimeTotalNum) << 1;// /128 * 256
            unsigned long ulPreSectionTime256T = ((unsigned long)wAccelStep) << 1;// /128 * 256
            unsigned long ulMainTime256T = ulTotalTime256T - ulPreSectionTime256T;
            long lDivider1 = ulPreSectionTime256T + (ulMainTime256T << 1);
            long lDivider2 = (ulMainTime256T << 1);

            if(lDivider1 == 0)
                lDivider1 = 1;

            if(lDivider2 == 0)
                lDivider2 = 1;

            for( bID=MIN_SERVO_ID; bID<NUMBER_OF_JOINTS; bID++ )
			{
            		long lStartSpeed1024_PreTime_256T = (long)ipLastOutSpeed1024[bID] * ulPreSectionTime256T; //  *300/1024 * 1024/720 * 256 * 2
            		long lMovingAngle_Speed1024Scale_256T_2T = (((long)ipMovingAngle1024[bID]) * 2560L) / 12;

					if( bpFinishType[bID] == ZERO_FINISH )
						ipMainSpeed1024[bID] = (short int)((lMovingAngle_Speed1024Scale_256T_2T - lStartSpeed1024_PreTime_256T) / lDivider2);
					else
						ipMainSpeed1024[bID] = (short int)((lMovingAngle_Speed1024Scale_256T_2T - lStartSpeed1024_PreTime_256T) / lDivider1);

					if( ipMainSpeed1024[bID] > 1023 )
						ipMainSpeed1024[bID] = 1023;

					if( ipMainSpeed1024[bID] < -1023 )
						ipMainSpeed1024[bID] = -1023;
            }

            wUnitTimeNum = wAccelStep; //PreSection
        }
    }
}
