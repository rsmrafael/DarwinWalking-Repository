/*
 * motioneditor.c
 *
 *  Created on: 01.03.2012
 *      Author: Stefan
 */

#include "tests/motioneditor.h"
#include "movements/action.h"
#include "dbgu/dbgu.h"
#include "tc/tc.h"
#include "ui/ui.h"
#include "servo/servo.h"
#include "eeprom/eeprom.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#endif

#define SCREEN_COL			80
#define SCREEN_ROW			24

// Position of Column
#define CMD_COL			2
#define STP7_COL		19
#define STP0_COL		25
#define STP1_COL		30
#define STP2_COL		35
#define STP3_COL		40
#define STP4_COL		45
#define STP5_COL		50
#define STP6_COL		55
#define CWSLOPE_COL		60
#define CCWSLOPE_COL	61
#define NAME_COL		63
#define ADDR_COL		72
#define PAGENUM_COL		75
#define PAGEPARAM_COL	76


// Position of Row
#define ID_1_ROW	0
#define ID_2_ROW	1
#define ID_3_ROW	2
#define ID_4_ROW	3
#define ID_5_ROW	4
#define ID_6_ROW	5
#define ID_7_ROW	6
#define ID_8_ROW	7
#define ID_9_ROW	8
#define ID_10_ROW	9
#define ID_11_ROW	10
#define ID_12_ROW	11
#define ID_13_ROW	12
#define ID_14_ROW	13
#define ID_15_ROW	14
#define ID_16_ROW	15
#define ID_17_ROW	16
#define ID_18_ROW	17
#define ID_19_ROW	18
#define ID_20_ROW	19
#define PAUSE_ROW	20
#define SPEED_ROW	21
#define CMD_ROW		23
#define NAME_ROW		0
#define PAGENUM_ROW		1
#define ADDR_ROW		2
#define PLAYCOUNT_ROW	3
#define STEPNUM_ROW		4
#define PLAYSPEED_ROW	5
#define ACCEL_ROW		6
#define NEXT_ROW		7
#define EXIT_ROW		8

#define MOTION_ANGLE_TO_VAL1024(angle)	lroundf(-angle * 3.413) + 512 // 1024 / 300 = 3.413
#define MOTION_VAL1024_TO_ANGLE(value)	-(value - 512) * 0.293 // 300 / 1024 = 0.293

int MotionEditor_Col = STP7_COL;
int MotionEditor_Row = ID_1_ROW;
int MotionEditor_Old_Col;
int MotionEditor_Old_Row;
int MotionEditor_indexPage = 1;
unsigned char MotionEditor_bBeginCommandMode = 0;
unsigned char MotionEditor_bEdited = 0;
PAGE Page;
STEP Step;



void MotionEditor_ClearScreen(void) {
#ifdef WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = {0, 0};
	DWORD dw;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwSize;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, ' ', dwSize, coord, &dw);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwSize, coord, &dw);
	SetConsoleCursorPosition(hConsole, coord);
#else
	dbgu_printf("\x1B[2J");
#endif
}

void MotionEditor_GoToCursor(int col, int row) {
#ifdef WIN32
	HANDLE screen_buffer_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(screen_buffer_handle, coord);
#else
	dbgu_printf("\x1B[%d;%dH", row + 1, col + 1);
#endif
	MotionEditor_Col = col;
	MotionEditor_Row = row;
}

void MotionEditor_PrintCmd(const char *message) {
	int len = strlen(message);
	MotionEditor_GoToCursor(0, CMD_ROW);

	dbgu_printf("] %s", message);
	for(int i = 0; i < (SCREEN_COL - (len + 2)); i++)
		dbgu_printf(" ");

	MotionEditor_GoToCursor(len + 2, CMD_ROW);
}

void MotionEditor_ReadStep()
{
	for(int id=0; id<31; id++)
	{
		if(id >= MIN_SERVO_ID && id <= NUMBER_OF_JOINTS)
		{
			if (Servo_GetPassive(id) == 0) {
				float value = Servo_ReadPos(id);
				if(value != -1000) {
					Step.position[id] = MOTION_ANGLE_TO_VAL1024(value);
				} else {
					Step.position[id] = INVALID_BIT_MASK;
				}
			} else {
				Step.position[id] = TORQUE_OFF_BIT_MASK;
			}
		}
		else
			Step.position[id] = INVALID_BIT_MASK;
	}
}
void MotionEditor_DrawName()
{
	int old_col = MotionEditor_Col;
	int old_row = MotionEditor_Row;

	MotionEditor_GoToCursor(NAME_COL, NAME_ROW);
	dbgu_printf( "                " );
	MotionEditor_GoToCursor(NAME_COL, NAME_ROW);

	for(int i=0; i<MAXNUM_NAME; i++)
		dbgu_printf("%c", (char)Page.header.name[i]);

	MotionEditor_GoToCursor( old_col, old_row );
}

void MotionEditor_ClearCmd() {
	MotionEditor_PrintCmd("");
}
void MotionEditor_DrawStep(int stepIndex)
{
	int old_col = MotionEditor_Col;
	int old_row = MotionEditor_Row;
	STEP *step;
	int col;

	switch(stepIndex)
	{
	case 0:
		col = STP0_COL;
		step = &Page.step[0];
		break;

	case 1:
		col = STP1_COL;
		step = &Page.step[1];
		break;

	case 2:
		col = STP2_COL;
		step = &Page.step[2];
		break;

	case 3:
		col = STP3_COL;
		step = &Page.step[3];
		break;

	case 4:
		col = STP4_COL;
		step = &Page.step[4];
		break;

	case 5:
		col = STP5_COL;
		step = &Page.step[5];
		break;

	case 6:
		col = STP6_COL;
		step = &Page.step[6];
		break;

	case 7:
		col = STP7_COL;
		step = &Step;
		break;

	default:
		return;
	}

	for( int id= MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++)
	{
		MotionEditor_GoToCursor(col, id -1);
		if(step->position[id] & INVALID_BIT_MASK)
			dbgu_printf("----");
		else if(step->position[id] & TORQUE_OFF_BIT_MASK)
			dbgu_printf("?\?\?\?");
		else
			dbgu_printf("%4d", step->position[id]);
	}

	MotionEditor_GoToCursor(col, PAUSE_ROW);
	dbgu_printf("%4d", step->pause);

	MotionEditor_GoToCursor(col, SPEED_ROW);
	dbgu_printf("%4d", step->time);

	MotionEditor_GoToCursor( old_col, old_row );
}

void MotionEditor_DrawStepLine(unsigned char erase) {
	int old_col = MotionEditor_Col;
	int old_row = MotionEditor_Row;
	int col;

		col = STP0_COL;

	col--;

	for(int id=0; id<(20 + 2); id++) {
		MotionEditor_GoToCursor(col, id - 1);
		if(erase == 1) {
			dbgu_printf( " " );
		} else {
			dbgu_printf( "|" );
		}
	}

	MotionEditor_GoToCursor(old_col, old_row);
}

void MotionEditor_MoveUpCursor() {
	if(MotionEditor_Col >= STP7_COL && MotionEditor_Col <= CCWSLOPE_COL) {
		if( MotionEditor_Row > ID_1_ROW )
			//dbgu_print_ascii("Up");
			MotionEditor_GoToCursor(MotionEditor_Col, MotionEditor_Row-1);
	} else {
		if( MotionEditor_Row > PLAYCOUNT_ROW )
			MotionEditor_GoToCursor(MotionEditor_Col, MotionEditor_Row-1);
	}
}

void MotionEditor_MoveDownCursor() {
	if(MotionEditor_Col >= STP7_COL && MotionEditor_Col <= STP6_COL) {
		if( MotionEditor_Row < SPEED_ROW )
			MotionEditor_GoToCursor(MotionEditor_Col, MotionEditor_Row+1);
	} else if(MotionEditor_Col <= CCWSLOPE_COL) {
		if( MotionEditor_Row < ID_20_ROW )
			MotionEditor_GoToCursor(MotionEditor_Col, MotionEditor_Row+1);
	} else {
		if( MotionEditor_Row < EXIT_ROW )
			MotionEditor_GoToCursor(MotionEditor_Col, MotionEditor_Row+1);
	}
}

void MotionEditor_MoveLeftCursor() {
	switch(MotionEditor_Col) {
	case STP0_COL:
		MotionEditor_GoToCursor(STP7_COL, MotionEditor_Row);
		break;

	case STP1_COL:
		MotionEditor_GoToCursor(STP0_COL, MotionEditor_Row);
		break;

	case STP2_COL:
		MotionEditor_GoToCursor(STP1_COL, MotionEditor_Row);
		break;

	case STP3_COL:
		MotionEditor_GoToCursor(STP2_COL, MotionEditor_Row);
		break;

	case STP4_COL:
		MotionEditor_GoToCursor(STP3_COL, MotionEditor_Row);
		break;

	case STP5_COL:
		MotionEditor_GoToCursor(STP4_COL, MotionEditor_Row);
		break;

	case STP6_COL:
		MotionEditor_GoToCursor(STP5_COL, MotionEditor_Row);
		break;

	case CWSLOPE_COL:
		MotionEditor_GoToCursor(STP6_COL, MotionEditor_Row);
		break;

	case CCWSLOPE_COL:
		MotionEditor_GoToCursor(CWSLOPE_COL, MotionEditor_Row);
		break;

	case PAGEPARAM_COL:
		MotionEditor_GoToCursor(CCWSLOPE_COL, MotionEditor_Row);
		break;
	}
}

void MotionEditor_MoveRightCursor() {
	switch(MotionEditor_Col) {
	case STP7_COL:
		MotionEditor_GoToCursor(STP0_COL, MotionEditor_Row);
		break;

	case STP0_COL:
		MotionEditor_GoToCursor(STP1_COL, MotionEditor_Row);
		break;

	case STP1_COL:
		MotionEditor_GoToCursor(STP2_COL, MotionEditor_Row);
		break;

	case STP2_COL:
		MotionEditor_GoToCursor(STP3_COL, MotionEditor_Row);
		break;

	case STP3_COL:
		MotionEditor_GoToCursor(STP4_COL, MotionEditor_Row);
		break;

	case STP4_COL:
		MotionEditor_GoToCursor(STP5_COL, MotionEditor_Row);
		break;

	case STP5_COL:
		MotionEditor_GoToCursor(STP6_COL, MotionEditor_Row);
		break;

	case STP6_COL:
		MotionEditor_GoToCursor(CWSLOPE_COL, MotionEditor_Row);
		break;

	case CWSLOPE_COL:
		MotionEditor_GoToCursor(CCWSLOPE_COL, MotionEditor_Row);
		break;

	case CCWSLOPE_COL:
		if(MotionEditor_Row >= PLAYCOUNT_ROW && MotionEditor_Row <= EXIT_ROW)
			MotionEditor_GoToCursor(PAGEPARAM_COL, MotionEditor_Row);
		break;
	}
}

void MotionEditor_DrawPage() {
	int old_col = MotionEditor_Col;
	int old_row = MotionEditor_Row;

	MotionEditor_ClearScreen();
	MotionEditor_GoToCursor(0, 0);

	// 80         01234567890123456789012345678901234567890123456789012345678901234567890123456789     //24
	dbgu_printf( "ID: 1(R_SHO_PITCH)[    ]                                                       \n\r" );//0
	dbgu_printf( "ID: 2(L_SHO_PITCH)[    ]                                       Page Number:    \n\r" );//1
	dbgu_printf( "ID: 3(R_SHO_ROLL) [    ]                                        Address:       \n\r" );//2
	dbgu_printf( "ID: 4(L_SHO_ROLL) [    ]                                         Play Count:   \n\r" );//3
	dbgu_printf( "ID: 5(R_ELBOW)    [    ]                                          Page Step:   \n\r" );//4
	dbgu_printf( "ID: 6(L_ELBOW)    [    ]                                         Page Speed:   \n\r" );//5
	dbgu_printf( "ID: 7(R_HIP_YAW)  [    ]                                         Accel Time:   \n\r" );//6
	dbgu_printf( "ID: 8(L_HIP_YAW)  [    ]                                       Link to Next:   \n\r" );//7
	dbgu_printf( "ID: 9(R_HIP_ROLL) [    ]                                       Link to Exit:   \n\r" );//8
	dbgu_printf( "ID:10(L_HIP_ROLL) [    ]                                                       \n\r" );//9
	dbgu_printf( "ID:11(R_HIP_PITCH)[    ]                                                       \n\r" );//0
	dbgu_printf( "ID:12(L_HIP_PITCH)[    ]                                                       \n\r" );//1
	dbgu_printf( "ID:13(R_KNEE)     [    ]                                                       \n\r" );//2
	dbgu_printf( "ID:14(L_KNEE)     [    ]                                                       \n\r" );//3
	dbgu_printf( "ID:15(R_ANK_PITCH)[    ]                                                       \n\r" );//4
	dbgu_printf( "ID:16(L_ANK_PITCH)[    ]                                                       \n\r" );//5
	dbgu_printf( "ID:17(R_ANK_ROLL) [    ]                                                       \n\r" );//6
	dbgu_printf( "ID:18(L_ANK_ROLL) [    ]                                                       \n\r" );//7
	dbgu_printf( "ID:19(HEAD_PAN)   [    ]                                                       \n\r" );//8
	dbgu_printf( "ID:20(HEAD_TILT)  [    ]                                                       \n\r" );//9
	dbgu_printf( "   PauseTime      [    ]                                                       \n\r" );//0

	if( Page.header.schedule == SPEED_BASE_SCHEDULE )
		dbgu_printf( "   Speed          [    ]                                                       \n\r" );//1
	else if( Page.header.schedule == TIME_BASE_SCHEDULE )
		dbgu_printf( "   Time(x 8msec)  [    ]                                                       \n\r" );//1

	dbgu_printf( "                   STP7  STP0 STP1 STP2 STP3 STP4 STP5 STP6                    \n\r" );//2
	dbgu_printf( "]                                                                              " );  // 3

	MotionEditor_ClearCmd();

	for(int i=0; i<=MAXNUM_STEP; i++ )
	{
		MotionEditor_DrawStep(i);
	}

	// Draw Compliance slope
	for( int id= MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++ ) {
		MotionEditor_GoToCursor(CWSLOPE_COL, id -1);
		dbgu_printf( "%1d%1d", Page.header.slope[id]>>4, Page.header.slope[id]&0x0f );
	}


	// Draw Page parameter
	MotionEditor_GoToCursor( PAGEPARAM_COL, PLAYCOUNT_ROW );
	dbgu_printf( "%3d", Page.header.repeat );

	MotionEditor_GoToCursor( PAGEPARAM_COL, STEPNUM_ROW );
	dbgu_printf( "%3d", Page.header.stepnum );

	MotionEditor_GoToCursor( PAGEPARAM_COL, PLAYSPEED_ROW );
	dbgu_printf( "%3d", Page.header.speed );

	MotionEditor_GoToCursor( PAGEPARAM_COL, ACCEL_ROW );
	dbgu_printf( "%3d", Page.header.accel );

	MotionEditor_GoToCursor( PAGEPARAM_COL, NEXT_ROW );
	dbgu_printf( "%3d", Page.header.next );

	MotionEditor_GoToCursor( PAGEPARAM_COL, EXIT_ROW );
	dbgu_printf( "%3d", Page.header.exit );

	// Draw Page information
	MotionEditor_DrawName();

	MotionEditor_GoToCursor(PAGENUM_COL, PAGENUM_ROW);
	dbgu_printf( "%4d", MotionEditor_indexPage );

	MotionEditor_GoToCursor(ADDR_COL, ADDR_ROW);
	dbgu_printf( "0x%5X", (int)(MotionEditor_indexPage*sizeof(PAGE)) );

	MotionEditor_DrawStepLine(0);

	MotionEditor_GoToCursor(old_col, old_row);
}


void MotionEditor_BeginCommandMode() {
	MotionEditor_Old_Col = MotionEditor_Col;
	MotionEditor_Old_Row = MotionEditor_Row;
	MotionEditor_ClearCmd();
	MotionEditor_GoToCursor(CMD_COL, CMD_ROW);
	MotionEditor_bBeginCommandMode = 1;
}


void MotionEditor_EndCommandMode() {
	MotionEditor_GoToCursor(MotionEditor_Old_Col, MotionEditor_Old_Row);
	MotionEditor_bBeginCommandMode = 0;
}

unsigned char MotionEditor_AskSave() {
	if(MotionEditor_bEdited == 1) {
		MotionEditor_PrintCmd("Are you sure? (y/n)");
		if(dbgu_get_char() != 'y') {
			MotionEditor_ClearCmd();
			return 1;
		}
	}

	return 0;
}

void MotionEditor_HelpCmd()
    {
		MotionEditor_ClearScreen();
    	dbgu_printf(" exit               Exits the program.\n\r");
    	dbgu_printf(" re                 Refreshes the screen.\n\r");
    	dbgu_printf(" b                  Move to previous page.\n\r");
    	dbgu_printf(" n                  Move to next page.\n\r");
    	dbgu_printf(" page [index]       Move to page [index].\n\r");
    	dbgu_printf(" list               View list of pages.\n\r");
    	dbgu_printf(" new                Clears data of current page and initializes page.\n\r");
    	dbgu_printf(" copy [index]       Copy data from page [index].\n\r");
    	dbgu_printf(" set [value]        Sets value on cursor [value].\n\r");
    	dbgu_printf(" save               Saves changes.\n\r");
    	dbgu_printf(" play               Motion playback of current page.\n\r");
    	dbgu_printf(" g [index]          Motion playback of STP[index].\n\r");
    	dbgu_printf(" name               Name for current page or changes the name of current page.\n\r");
    	dbgu_printf(" time               Change time base playing.\n\r");
    	dbgu_printf(" speed              Change speed base playing.\n\r");
    	dbgu_printf(" w [index]          Overwrites data from STP[index] with STP7.\n\r");
    	dbgu_printf(" i                  Inserts data from STP7 to STP0. \n\r" \
    	       "                    Moves data from STP[x] to STP[x+1].\n\r");
    	dbgu_printf(" i [index]          Inserts data from STP7 to STP[index]. \n\r" \
    	       "                    Moves data from STP[index] to STP[index+1].\n\r");
    	dbgu_printf(" m [index] [index2] Moves data from [index] to [index2] step.\n\r");
    	dbgu_printf(" d [index]          Deletes data from STP[index]. \n\r"
    	       "                    Pushes data from STP[index] to STP[index-1].\n\r");
    	dbgu_printf(" on/off             Turn On/Off torque from ALL actuators.\n\r");
    	dbgu_printf(" on/off [index1] [index2] ...  \n\r"
    	       "                    turns On/Off torque from ID[index1] ID[index2]...\n\r");
    	dbgu_printf("\n\r");
    	dbgu_printf(" Press any key to continue...");
    	dbgu_get_char();

    	MotionEditor_DrawPage();
    }
void MotionEditor_SpeedCmd()
{
	Page.header.schedule = SPEED_BASE_SCHEDULE;
	MotionEditor_bEdited = 1;
	MotionEditor_DrawPage();
}

void MotionEditor_PageCmd(int in)
{
		if(MotionEditor_AskSave() == 1)
			return;

		if(in > 0 && in < MAXNUM_PAGE)
		{
			MotionEditor_indexPage = in;
			Action_LoadPage(MotionEditor_indexPage, &Page);

			MotionEditor_Col = STP7_COL;
			MotionEditor_Row = ID_1_ROW;
			MotionEditor_DrawPage();
		}
		else
			dbgu_printf("Invalid page index");

		MotionEditor_bEdited = 0;
}
void MotionEditor_TimeCmd()
{
	Page.header.schedule = TIME_BASE_SCHEDULE;
	MotionEditor_bEdited = 1;
	MotionEditor_DrawPage();
}
void MotionEditor_NextCmd()
	{
		MotionEditor_PageCmd(MotionEditor_indexPage + 1);
	}
void MotionEditor_PrevCmd()
{
	MotionEditor_PageCmd(MotionEditor_indexPage - 1);
}
void MotionEditor_ListCmd()
{
	int old_col = MotionEditor_Col;
	int old_row = MotionEditor_Row;
	int in = 0;

	while(1)
	{
		MotionEditor_ClearScreen();
		for(int i=0; i<22; i++)
		{
			for(int j=0; j<4; j++)
			{
				int k = (in * 88) + (j*22 + i);
				PAGE page;
				if(Action_LoadPage(k, &page) == 1)
				{
					dbgu_printf(" |%3d.", k);
					for (int n = 0; n < MAXNUM_NAME; n++) {
						if((char)page.header.name[n] >= ' ' && (char)page.header.name[n] <= '~')
							dbgu_printf("%c", (char)page.header.name[n]);
						else
							dbgu_printf(" ");
					}
				}
				else
				{
					dbgu_printf(" |                ");
				}
			}
			dbgu_printf("\n\r");
		}

		dbgu_printf("\n\rAction Page List (%d/3) - Press key n(Next), b(Prev), q(Quit)", in + 1);
		while(1)
		{
			int ch = dbgu_get_char();
			if(ch == 'n')
			{
				if(in < 2)
				{
					in++;
					break;
				}
			}
			else if(ch == 'b')
			{
				if(in > 0)
				{
					in--;
					break;
				}
			}
			else if(ch == 'q')
			{
				MotionEditor_DrawPage();
				MotionEditor_GoToCursor(old_col, old_row);
				return;
			}
		}
	}
}
void MotionEditor_NameCmd()
   {
   	MotionEditor_ClearCmd();
   	MotionEditor_GoToCursor(CMD_COL, CMD_ROW);
   	dbgu_printf("name: ");
   	char name[80] = {0};
   	dbgu_gets(name,20);
   	for(int i=0; i<=MAXNUM_NAME; i++)
   		Page.header.name[i] = name[i];
   	MotionEditor_DrawName();
   	MotionEditor_bEdited = 1;
   }
void MotionEditor_SaveCmd()
{
	if(MotionEditor_bEdited == 0)
		return;

	if(Action_SavePage(MotionEditor_indexPage, &Page) == 1)
		MotionEditor_bEdited = 0;
}
void MotionEditor_CopyCmd(int in)
{
	if(in == MotionEditor_indexPage)
		return;

	if(Action_LoadPage(in, &Page) == 1)
	{
		MotionEditor_DrawPage();
		MotionEditor_bEdited = 1;
	}
	else
		dbgu_printf("Invalid page index");
}
void MotionEditor_NewCmd()
{
	Action_ResetPage(&Page);
	MotionEditor_DrawPage();
	MotionEditor_bEdited = 1;
}
void MotionEditor_WriteStepCmd(int in)
{
	for( int id= MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++)
	{
		if(Step.position[id] & TORQUE_OFF_BIT_MASK)
			return;
	}

	if(in >= 0 && in < MAXNUM_STEP)
	{
		Page.step[in] = Step;
		MotionEditor_DrawStep(in);
		MotionEditor_bEdited = 1;
	}
	else
		MotionEditor_PrintCmd("Invalid step index");
}

void MotionEditor_DeleteStepCmd(int in)
{
	if(in >= 0 && in < MAXNUM_STEP)
	{
		for(int i=in; i<MAXNUM_STEP; i++)
		{
			if(i == (MAXNUM_STEP - 1))
			{
				for( int id= MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++)
					Page.step[i].position[id] = INVALID_BIT_MASK;

				Page.step[i].pause = 0;
				Page.step[i].time = 0;
			}
			else
				Page.step[i] = Page.step[i + 1];

			MotionEditor_DrawStep(i);
		}

		if(in < Page.header.stepnum)
		{
			if(Page.header.stepnum != 0)
			{
				MotionEditor_DrawStepLine(1);
				Page.header.stepnum--;
				MotionEditor_DrawStepLine(0);
			}

			MotionEditor_GoToCursor(PAGEPARAM_COL, STEPNUM_ROW);
			dbgu_printf( "%3d", Page.header.stepnum );
		}

		MotionEditor_bEdited = 1;
	}
	else
		MotionEditor_PrintCmd("Invalid step index");
}

void MotionEditor_InsertStepCmd(int in)
{
	for( int id= MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++)
	{
		if(Step.position[id] & TORQUE_OFF_BIT_MASK)
		{
			MotionEditor_PrintCmd("Exist invalid joint value");
			return;
		}
	}

	if(in >= 0 && in< MAXNUM_STEP)
	{
		for(int i=MAXNUM_STEP-1; i>in; i-- )
		{
			Page.step[i] = Page.step[i-1];
			MotionEditor_DrawStep(i);
		}

		Page.step[in] = Step;
		MotionEditor_DrawStep(in);

		if(in == 0 || in <= Page.header.stepnum)
		{
			if(Page.header.stepnum != MAXNUM_STEP)
			{
				MotionEditor_DrawStepLine(1);
				Page.header.stepnum++;
				MotionEditor_DrawStepLine(0);
			}

			MotionEditor_GoToCursor(PAGEPARAM_COL, STEPNUM_ROW);
			dbgu_printf( "%3d", Page.header.stepnum );
		}

		MotionEditor_bEdited = 1;
	}
	else
		MotionEditor_PrintCmd("Invalid step index");
}
void MotionEditor_MoveStepCmd(int src, int dst)
{
	if(src < 0 || src >= MAXNUM_STEP)
	{
		MotionEditor_PrintCmd("Invalid step index");
		return;
	}

	if(dst < 0 || dst >= MAXNUM_STEP)
	{
		MotionEditor_PrintCmd("Invalid step index");
		return;
	}

	if(src == dst)
		return;

	STEP step = Page.step[src];
	if(src < dst)
	{
		for(int i=src; i<dst; i++)
		{
			Page.step[i] = Page.step[i + 1];
			MotionEditor_DrawStep(i);
		}
	}
	else
	{
		for(int i=src; i>dst; i--)
		{
			Page.step[i] = Page.step[i - 1];
			MotionEditor_DrawStep(i);
		}
	}

	Page.step[dst] = step;
	MotionEditor_DrawStep(dst);
	MotionEditor_bEdited = 1;
}


void MotionEditor_SetValue(int value)
{
	int col;
	int row;
	if(MotionEditor_bBeginCommandMode == 1)
	{
		col = MotionEditor_Old_Col;
		row = MotionEditor_Old_Row;
	}
	else
	{
		col = MotionEditor_Col;
		row = MotionEditor_Row;
	}

	MotionEditor_GoToCursor(col, row);

	if( col == STP7_COL )
	{
		if( row == PAUSE_ROW )
		{
			if(value >= 0 && value <= 255)
			{
				Step.pause = value;
				dbgu_printf( "%4d", value );
				MotionEditor_bEdited = 1;
			}
		}
		else if( row == SPEED_ROW )
		{
			if(value >= 0 && value <= 255)
			{
				Step.time = value;
				dbgu_printf( "%4d", value );
				MotionEditor_bEdited = 1;
			}
		}
		else
		{
			if(value >= 0 && value <= 1023)
			{
				if(!(Step.position[row + 1] & INVALID_BIT_MASK) && !(Step.position[row + 1] & TORQUE_OFF_BIT_MASK))
				{
					if(Servo_SendPosition(row+1,MOTION_VAL1024_TO_ANGLE(value))>=1)
					{
						Step.position[row + 1] = value;
						dbgu_printf( "%4d", value );
						MotionEditor_bEdited = 1;
					}
				}
			}
		}
	}
	else if( col <= STP6_COL )
	{
		int i=0;
		switch(col)
		{
		case STP0_COL:
			i = 0;
			break;

		case STP1_COL:
			i = 1;
			break;

		case STP2_COL:
			i = 2;
			break;

		case STP3_COL:
			i = 3;
			break;

		case STP4_COL:
			i = 4;
			break;

		case STP5_COL:
			i = 5;
			break;

		case STP6_COL:
			i = 6;
			break;
		}

		if( row == PAUSE_ROW )
		{
			if(value >= 0 && value <= 255)
			{
				Page.step[i].pause = value;
				dbgu_printf( "%4d", value );
				MotionEditor_bEdited = 1;
			}
		}
		else if( row == SPEED_ROW )
		{
			if(value >= 0 && value <= 255)
			{
				Page.step[i].time = value;
				dbgu_printf( "%4d", value );
				MotionEditor_bEdited = 1;
			}
		}
		else
		{
			if(value >= 0 && value <= 1023)
			{
				if(!(Page.step[i].position[row + 1] & INVALID_BIT_MASK))
				{
					Page.step[i].position[row + 1] = value;
					dbgu_printf( "%4d", value );
					MotionEditor_bEdited = 1;
				}
			}
		}
	}
	else if(col == CWSLOPE_COL)
	{
		if(value >= 1 && value <= 7)
		{
			Page.header.slope[row + 1] = (value << 4) + (Page.header.slope[row + 1] & 0x0f);
			dbgu_printf( "%1d", value );
			MotionEditor_bEdited = 1;
		}
	}
	else if(col == CCWSLOPE_COL)
	{
		if(value >= 1 && value <= 7)
		{
			Page.header.slope[row + 1] = (Page.header.slope[row + 1] & 0xf0) + (value & 0x0f);
			dbgu_printf( "%1d", value );
			MotionEditor_bEdited = 1;
		}
	}
	else if(col == PAGEPARAM_COL)
	{
		if(row == PLAYCOUNT_ROW)
		{
			if(value >= 0 && value <= 255)
			{
				Page.header.repeat = value;
				dbgu_printf( "%3d", value );
				MotionEditor_bEdited = 1;
			}
		}
		else if(row == STEPNUM_ROW)
		{
			if(value >= 0 && value <= MAXNUM_STEP)
			{
				if(Page.header.stepnum != value)
				{
					MotionEditor_DrawStepLine(1);
					Page.header.stepnum = value;
					MotionEditor_DrawStepLine(0);
					dbgu_printf( "%3d", value );
					MotionEditor_bEdited = 1;
				}
			}
		}
		else if(row == PLAYSPEED_ROW)
		{
			if(value >= 0 && value <= 255)
			{
				Page.header.speed = value;
				dbgu_printf( "%3d", value );
				MotionEditor_bEdited = 1;
			}
		}
		else if(row == ACCEL_ROW)
		{
			if(value >= 0 && value <= 255)
			{
				Page.header.accel = value;
				dbgu_printf( "%3d", value );
				MotionEditor_bEdited = 1;
			}
		}
		else if(row == NEXT_ROW)
		{
			if(value >= 0 && value <= 255)
			{
				Page.header.next = value;
				dbgu_printf( "%3d", value );
				MotionEditor_bEdited = 1;
			}
		}
		else if(row == EXIT_ROW)
		{
			if(value >= 0 && value <= 255)
			{
				Page.header.exit = value;
				dbgu_printf( "%3d", value );
				MotionEditor_bEdited = 1;
			}
		}
	}

	MotionEditor_GoToCursor(col, row);
}
int MotionEditor_GetValue()
{
	int col;
	int row;
	if(MotionEditor_bBeginCommandMode == 1)
	{
		col = MotionEditor_Old_Col;
		row = MotionEditor_Old_Row;
	}
	else
	{
		col = MotionEditor_Col;
		row = MotionEditor_Row;
	}

	if( col == STP7_COL )
	{
		if( row == PAUSE_ROW )
			return Step.pause;
		else if( row == SPEED_ROW )
			return Step.time;
		else
			return Step.position[row + 1];
	}
	else if( col <= STP6_COL )
	{
		int i = 0;
		switch(col)
		{
		case STP0_COL:
			i = 0;
			break;

		case STP1_COL:
			i = 1;
			break;

		case STP2_COL:
			i = 2;
			break;

		case STP3_COL:
			i = 3;
			break;

		case STP4_COL:
			i = 4;
			break;

		case STP5_COL:
			i = 5;
			break;

		case STP6_COL:
			i = 6;
			break;
		}

		if( row == PAUSE_ROW )
			return Page.step[i].pause;
		else if( row == SPEED_ROW )
			return Page.step[i].time;
		else
			return Page.step[i].position[row + 1];
	}
	else if(col == CWSLOPE_COL)
		return (Page.header.slope[row + 1] >> 4);
	else if(col == CCWSLOPE_COL)
		return (Page.header.slope[row + 1] & 0x0f);
	else if(col == PAGEPARAM_COL)
	{
		if(row == PLAYCOUNT_ROW)
			return Page.header.repeat;
		else if(row == STEPNUM_ROW)
			return Page.header.stepnum;
		else if(row == PLAYSPEED_ROW)
			return Page.header.speed;
		else if(row == ACCEL_ROW)
			return Page.header.accel;
		else if(row == NEXT_ROW)
			return Page.header.next;
		else if(row == EXIT_ROW)
			return Page.header.exit;
	}

	return -1;
}

void MotionEditor_UpDownValue( int offset)
{
	MotionEditor_SetValue(MotionEditor_GetValue() + offset);
}
void MotionEditor_ToggleTorque()
{
	if(MotionEditor_Col != STP7_COL || MotionEditor_Row > ID_19_ROW)
		return;

	int id = MotionEditor_Row + 1;

	if(Step.position[id] & TORQUE_OFF_BIT_MASK)
	{

		if(Servo_SetPassive(id, 0) >= 1)
			return;

		float value=Servo_ReadPos(id);
		if(value == -1000)
			return;

		Step.position[id] = MOTION_ANGLE_TO_VAL1024(value);
		dbgu_printf("%4d", Step.position[id]);
	}
	else
	{
		if(Servo_SetPassive(id, 1)>=1)
			return;

		Step.position[id] = TORQUE_OFF_BIT_MASK;
		dbgu_printf("?\?\?\?");
	}

	MotionEditor_GoToCursor(MotionEditor_Col, MotionEditor_Row);
}

void MotionEditor_GoCmd(int in)
{
	if(in < 0 || in >= MAXNUM_STEP)
	{
		MotionEditor_PrintCmd("Invalid step index");
		return;
	}

	if(in > Page.header.stepnum)
	{
		MotionEditor_PrintCmd("Are you sure? (y/n)");
		if(dbgu_get_char() != 'y')
		{
			MotionEditor_ClearCmd();
			return;
		}
	}

	int id=0;
	float wGoalPosition;

	for( id= MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++)
	{
		if(Page.step[in].position[id] & INVALID_BIT_MASK)
		{
			MotionEditor_PrintCmd("Exist invalid joint value");
			return;
		}

		wGoalPosition = MOTION_VAL1024_TO_ANGLE(Page.step[in].position[id]);
		Servo_SetPosition(id, wGoalPosition);
	}
	Servo_SendPositions();

	Step = Page.step[in];
	MotionEditor_DrawStep(7);
}
void MotionEditor_PlayCmd()
{
	for(int i=0; i<Page.header.stepnum; i++)
	{
		for( int id= MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++)
		{
			if(Page.step[i].position[id] & INVALID_BIT_MASK)
			{
				MotionEditor_PrintCmd("Exist invalid joint value");
				return;
			}
		}
	}

	for(int id=MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++)
	{
			float value = Servo_ReadPos(id);
			if(value!=-1000)
				Servo_SetPosition(id, value);
	}
	Servo_SendPositions();

	MotionEditor_PrintCmd("Playing... ('s' to stop, 'b' to brake)");

	if(Action_Start_Page(MotionEditor_indexPage, &Page) == 0)
	{
		MotionEditor_PrintCmd("Failed to play this page!");
		return;
	}
	while(1)
	{
		if(Action_IsRunning()== 0)
			break;

		if(dbgu_char_available())
		{
			int key = dbgu_get_char();
			MotionEditor_GoToCursor(CMD_COL, CMD_ROW);
			if(key == 's')
			{
				Action_Stop();
				dbgu_printf("\r] Stopping...                                  ");
			}
			else if(key == 'b')
			{
				Action_Brake();
				dbgu_printf( "\r] Braking...                                   ");
			}
			else
				dbgu_printf("\r] Playing... ('s' to stop, 'b' to brake)");
		}

		TC_DelayMs(10);
	}

	MotionEditor_GoToCursor(CMD_COL, CMD_ROW);
	MotionEditor_PrintCmd("Done.");

	TC_DelayMs(10);

	MotionEditor_ReadStep();
	MotionEditor_DrawStep(7);
}
void MotionEditor_OnOffCmd( unsigned char on, int num_param, int *list)
{
	if(num_param == 0)
	{
		for( int id= MIN_SERVO_ID; id<NUMBER_OF_JOINTS; id++)
			Servo_SetPassive(id, on == 1 ? 0 : 1);
	}
	else
	{
		for(int i=0; i<num_param; i++)
		{
			if(list[i] >= MIN_SERVO_ID && list[i] <= NUMBER_OF_JOINTS)
				Servo_SetPassive(list[i], on == 1 ? 0 : 1);
		}
	}

	MotionEditor_ReadStep();
	MotionEditor_DrawStep(7);
}

void MotionEditor_Main(unsigned char motionIdx) {
	MotionEditor_Col = STP7_COL;
	MotionEditor_Row = ID_1_ROW;
	MotionEditor_bBeginCommandMode = 0;
	MotionEditor_bEdited = 0;

	MotionEditor_indexPage = motionIdx;
	Action_LoadPage(MotionEditor_indexPage, &Page);

	MotionEditor_ReadStep();
	Step.pause = 0;
	Step.time = 0;

	MotionEditor_DrawPage();

    while(1) {
		if (dbgu_char_available() == 1) {
			int ch = dbgu_get_char();
			//dbgu_printf("Char1: %x ", ch);
			if(ch == 0xe0 || ch == 0x1b) {
				ch = dbgu_get_char();
				//dbgu_printf("Char2: %x ", ch);
				if(ch==0x5b)
				{
					ch = dbgu_get_char();
					//dbgu_printf("Char3: %x ", ch);
					if(ch == 0x48||ch==0x41) // Up arrow key
						MotionEditor_MoveUpCursor();
					else if(ch == 0x50||ch==0x42) // Down arrow key
						MotionEditor_MoveDownCursor();
					else if(ch == 0x4b||ch==0x44) // Left arrow keyw
						MotionEditor_MoveLeftCursor();
					else if(ch == 0x4d||ch==0x43)
						MotionEditor_MoveRightCursor();
				}

			} else if( ch == '-' ) {
				MotionEditor_UpDownValue(-1);
			} else if( ch == '+' ) {
				MotionEditor_UpDownValue(1);
			} else if( ch == '_' ) {
				MotionEditor_UpDownValue(-10);
			} else if( ch == '*' ) {
				MotionEditor_UpDownValue(10);
			} else if( ch == ' ' ) {
				MotionEditor_ToggleTorque();
			} else if( ch >= 'A' && ch <= 'z' ) {
				char input[128] = {0,};
				char *token;
				int input_len;
				char cmd[80];
				int iparam[30];

				int idx = 0;

				MotionEditor_BeginCommandMode();

				dbgu_printf("%c", ch);
				input[idx++] = (char)ch;

				while(1) {
					ch = dbgu_get_char();
					if( ch == 0x0D || ch == 0x0A ) {
						break;
					} else if (ch == 0x08 || ch == 0x7F) {
						if(idx > 0) {
							ch = 0x08;
							dbgu_printf("%c", ch);
							ch = ' ';
							dbgu_printf("%c", ch);
							ch = 0x08;
							dbgu_printf("%c", ch);
							input[--idx] = 0;
						}
					} else if( ( ch >= 'A' && ch <= 'z' ) || ch == ' ' || ( ch >= '0' && ch <= '9')) {
						if(idx < 127) {
							dbgu_printf("%c", ch);
							input[idx++] = (char)ch;
						}
					}
				}

				//fflush(stdin);
				input_len = strlen(input);
				if(input_len > 0) {
					token = strtok( input, " " );
					if(token != 0) {
						strcpy( cmd, token );
						token = strtok( 0, " " );
						int num_param = 0;
						while(token != 0) {
							iparam[num_param++] = atoi(token);
							num_param++;
							token = strtok( 0, " " );
						}

						if(strcmp(cmd, "exit") == 0) {
							if(MotionEditor_AskSave() == 0)
								break;
						} else if(strcmp(cmd, "re") == 0) {
							MotionEditor_DrawPage();
						} else if(strcmp(cmd, "help") == 0) {
							MotionEditor_HelpCmd();
						} else if(strcmp(cmd, "n") == 0) {
							MotionEditor_NextCmd();
						} else if(strcmp(cmd, "b") == 0) {
							MotionEditor_PrevCmd();
						} else if(strcmp(cmd, "time") == 0) {
							MotionEditor_TimeCmd();
						} else if(strcmp(cmd, "speed") == 0) {
							MotionEditor_SpeedCmd();
						} else if(strcmp(cmd, "page") == 0) {
							if(num_param > 0) {
								MotionEditor_PageCmd(iparam[0]);
							} else {
								MotionEditor_PrintCmd("Need parameter");
							}
						} else if(strcmp(cmd, "play") == 0) {
							MotionEditor_PlayCmd();
						} else if(strcmp(cmd, "set") == 0) {
							if(num_param > 0) {
								MotionEditor_SetValue(iparam[0]);
							} else {
								MotionEditor_PrintCmd("Need parameter");
							}
						} else if(strcmp(cmd, "list") == 0) {
							MotionEditor_ListCmd();
						} else if(strcmp(cmd, "on") == 0) {
							MotionEditor_OnOffCmd( 1, num_param, iparam);
						} else if(strcmp(cmd, "off") == 0) {
							MotionEditor_OnOffCmd(0, num_param, iparam);
						} else if(strcmp(cmd, "w") == 0) {
							if(num_param > 0) {
								MotionEditor_WriteStepCmd(iparam[0]);
							} else {
								MotionEditor_PrintCmd("Need parameter");
							}
						} else if(strcmp(cmd, "d") == 0) {
							if(num_param > 0) {
								MotionEditor_DeleteStepCmd(iparam[0]);
							} else {
								MotionEditor_PrintCmd("Need parameter");
							}
						} else if(strcmp(cmd, "i") == 0) {
							if(num_param == 0) {
								MotionEditor_InsertStepCmd(0);
							} else {
								MotionEditor_InsertStepCmd(iparam[0]);
							}
						} else if(strcmp(cmd, "m") == 0) {
							if(num_param > 1) {
								MotionEditor_MoveStepCmd(iparam[0], iparam[1]);
							} else {
								MotionEditor_PrintCmd("Need parameter");
							}
						} else if(strcmp(cmd, "copy") == 0) {
							if(num_param > 0) {
								MotionEditor_CopyCmd(iparam[0]);
							} else {
								MotionEditor_PrintCmd("Need parameter");
							}
						} else if(strcmp(cmd, "new") == 0) {
							MotionEditor_NewCmd();
						} else if(strcmp(cmd, "g") == 0) {
							if(num_param > 0) {
								MotionEditor_GoCmd(iparam[0]);
							} else {
								MotionEditor_PrintCmd("Need parameter");
							}
						} else if(strcmp(cmd, "save") == 0) {
							MotionEditor_SaveCmd();
						} else if(strcmp(cmd, "name") == 0) {
							MotionEditor_NameCmd();
						} else {
							MotionEditor_PrintCmd("Bad command! please input 'help'");
						}
					}
				}

				MotionEditor_EndCommandMode();
			}
		}
    }


}
