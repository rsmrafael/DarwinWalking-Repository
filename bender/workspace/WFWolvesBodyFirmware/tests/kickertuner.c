/*
 * kickertuner.c
 *
 *  Created on: 04.06.2013
 *      Author: Oliver
 */

#include "kickertuner.h"
#include "dbgu/dbgu.h"
#include "tc/tc.h"
#include "ui/ui.h"
#include "kicker/kicker.h"
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif

#ifdef DEBUG_X86
#define MAIN_LOOP_DURATION			24 // ms
#else
#define MAIN_LOOP_DURATION			8 // ms
#endif

#ifndef SCREEN_COL
#define SCREEN_COL		35
//#define CMD_COL		2
//#define PARAM_COL		27
#endif

// rows of output
enum {
    HEADING_ROW,				// 0
    BALANCE_ENABLE_ROW,			// 1
    BALANCE_R_Y_ROW,
    BALANCE_L_Y_ROW,			// 3
    BALANCE_RL_P_ROW,
    BALANCE_RL_I_ROW,			// 5
    BALANCE_RL_D_ROW,
    BALANCE_FB_P_ROW,			// 7
	BALANCE_FB_I_ROW,
	BALANCE_FB_D_ROW,			// 9
    OFFSET_L_HIP_PITCH_ROW,
    OFFSET_L_HIP_ROLL_ROW,		// 11
    OFFSET_L_ANKLE_PITCH_ROW,
    OFFSET_L_ANKLE_ROLL_ROW,	// 13
    OFFSET_R_HIP_PITCH_ROW,
	OFFSET_R_HIP_ROLL_ROW,		// 15
	OFFSET_R_ANKLE_PITCH_ROW,
	OFFSET_R_ANKLE_ROLL_ROW,	// 17
    RANGE_MULTIPLIER_LEG_ROW,
    RANGE_MULTIPLIER_HIP_ROW,	// 19
    SERVO_P_ROW,
    SERVO_I_ROW,				// 21
    SERVO_D_ROW,
    CMD_ROW,
};

// columns of output
enum {
	NAME_COL	= 0,
	PARAM_COL	= 24,
	USER_COL	= 33
};

int Kickertuner_Col = 0;
int Kickertuner_Row = 0;
int Kickertuner_Old_Col;
int Kickertuner_Old_Row;

extern float kicker_RangeMultiplierLeg;
extern float kicker_RangeMultiplierHip;
enum kicker_type kickertuner_type = KICKER_TYPE_MEDIUM;

void Kickertuner_GoToCursor(int col, int row) {
	dbgu_GoToCursor( col, row);
	Kickertuner_Col = col;
	Kickertuner_Row = row;
}

void Kickertuner_BeginCmd(void) {
	dbgu_GoToCursor(0, CMD_ROW);
	dbgu_printf("] ");
	for(int i = 0; i < (SCREEN_COL - 2); i++)
		dbgu_printf(" ");

	dbgu_GoToCursor(3, CMD_ROW);
}

void Kickertuner_PrintCmd(const char *message) {
	Kickertuner_BeginCmd();
	dbgu_printf("%s", message);
}

void Kickertuner_MoveCursorToRow( int row) {
	if( row >= HEADING_ROW + 1 && row <= CMD_ROW - 1) {
		Kickertuner_GoToCursor(USER_COL, Kickertuner_Row);
		dbgu_printf(" ");
		Kickertuner_Row = row;
		Kickertuner_GoToCursor(USER_COL, Kickertuner_Row);
		dbgu_printf("*");
	}
}

void Kickertuner_MoveCursorUp() {
	if( Kickertuner_Row > HEADING_ROW + 1) {
		Kickertuner_GoToCursor(USER_COL, Kickertuner_Row);
		dbgu_printf(" ");
		Kickertuner_GoToCursor(USER_COL, Kickertuner_Row-1);
		dbgu_printf("*");
	}
}

void Kickertuner_MoveCursorDown() {
	if(Kickertuner_Row < CMD_ROW - 1) {
		Kickertuner_GoToCursor(USER_COL, Kickertuner_Row);
		dbgu_printf(" ");
		Kickertuner_GoToCursor(USER_COL, Kickertuner_Row+1);
		dbgu_printf("*");
	}
}

void Kickertuner_PrintBalance() {
	dbgu_GoToCursor( PARAM_COL, BALANCE_FB_P_ROW);
#ifdef KICKER_USE_BALANCE
	dbgu_printf("%1.3f    ", kicker_Params.balance_fb_p_gain);
#else
	dbgu_printf("%3.1f    ", kicker_Params.balance_fb_p_gain);
#endif
	dbgu_GoToCursor( PARAM_COL, BALANCE_FB_I_ROW);
#ifdef KICKER_USE_BALANCE
	dbgu_printf("%1.3f    ", kicker_Params.balance_fb_i_gain);
#else
	dbgu_printf("%3.1f    ", kicker_Params.balance_fb_i_gain);
#endif
	dbgu_GoToCursor( PARAM_COL, BALANCE_FB_D_ROW);
#ifdef KICKER_USE_BALANCE
	dbgu_printf("%1.3f    ", kicker_Params.balance_fb_d_gain);
#else
	dbgu_printf("%3.1f    ", kicker_Params.balance_fb_d_gain);
#endif
	dbgu_GoToCursor( PARAM_COL, BALANCE_RL_P_ROW);
#ifdef KICKER_USE_BALANCE
	dbgu_printf("%1.3f    ", kicker_Params.balance_rl_p_gain);
#else
	dbgu_printf("%3.1f    ", kicker_Params.balance_rl_p_gain);
#endif
	dbgu_GoToCursor( PARAM_COL, BALANCE_RL_I_ROW);
#ifdef KICKER_USE_BALANCE
	dbgu_printf("%1.3f    ", kicker_Params.balance_rl_i_gain);
#else
	dbgu_printf("%3.1f    ", kicker_Params.balance_rl_i_gain);
#endif
	dbgu_GoToCursor( PARAM_COL, BALANCE_RL_D_ROW);
#ifdef KICKER_USE_BALANCE
	dbgu_printf("%1.3f    ", kicker_Params.balance_rl_d_gain);
#else
	dbgu_printf("%3.1f    ", kicker_Params.balance_rl_d_gain);
#endif
	dbgu_GoToCursor( PARAM_COL, BALANCE_R_Y_ROW);
	dbgu_printf("%d    ", Kicker_GetBalanceRightOffset());
	dbgu_GoToCursor( PARAM_COL, BALANCE_L_Y_ROW);
	dbgu_printf("%d    ", Kicker_GetBalanceLeftOffset());
	dbgu_GoToCursor( PARAM_COL, BALANCE_ENABLE_ROW);
	dbgu_printf("%d    ", kicker_Params.balance_enable);
}

void Kickertuner_PrintServo() {
	dbgu_GoToCursor( PARAM_COL, SERVO_P_ROW);
	dbgu_printf("%2.2f    ", kicker_ServoParams.servo_gain_p);
	dbgu_GoToCursor( PARAM_COL, SERVO_I_ROW);
	dbgu_printf("%2.2f    ", kicker_ServoParams.servo_gain_i);
	dbgu_GoToCursor( PARAM_COL, SERVO_D_ROW);
	dbgu_printf("%2.2f    ", kicker_ServoParams.servo_gain_d);
}

void Kickertuner_PrintOffsets() {
	dbgu_GoToCursor( PARAM_COL, OFFSET_L_HIP_PITCH_ROW);
	dbgu_printf("%d    ", kicker_OffsetParams.hip_pitch_offset_l);
	dbgu_GoToCursor( PARAM_COL, OFFSET_L_HIP_ROLL_ROW);
	dbgu_printf("%d    ", kicker_OffsetParams.hip_roll_offset_l);
	dbgu_GoToCursor( PARAM_COL, OFFSET_L_ANKLE_PITCH_ROW);
	dbgu_printf("%d    ", kicker_OffsetParams.ankle_pitch_offset_l);
	dbgu_GoToCursor( PARAM_COL, OFFSET_L_ANKLE_ROLL_ROW);
	dbgu_printf("%d    ", kicker_OffsetParams.ankle_roll_offset_l);
	dbgu_GoToCursor( PARAM_COL, OFFSET_R_HIP_PITCH_ROW);
	dbgu_printf("%d    ", kicker_OffsetParams.hip_pitch_offset_r);
	dbgu_GoToCursor( PARAM_COL, OFFSET_R_HIP_ROLL_ROW);
	dbgu_printf("%d    ", kicker_OffsetParams.hip_roll_offset_r);
	dbgu_GoToCursor( PARAM_COL, OFFSET_R_ANKLE_PITCH_ROW);
	dbgu_printf("%d    ", kicker_OffsetParams.ankle_pitch_offset_r);
	dbgu_GoToCursor( PARAM_COL, OFFSET_R_ANKLE_ROLL_ROW);
	dbgu_printf("%d    ", kicker_OffsetParams.ankle_roll_offset_r);
}

void Kickertuner_PrintRange() {
	dbgu_GoToCursor( PARAM_COL, RANGE_MULTIPLIER_LEG_ROW);
	dbgu_printf("%1.3f    ", kicker_RangeMultiplierLeg);
	dbgu_GoToCursor( PARAM_COL, RANGE_MULTIPLIER_HIP_ROW);
	dbgu_printf("%1.3f    ", kicker_RangeMultiplierHip);
}

void Kickertuner_Add( signed short val) {
	dbgu_GoToCursor( PARAM_COL, Kickertuner_Row);
	switch( Kickertuner_Row) {
	case BALANCE_ENABLE_ROW:
		kicker_Params.balance_enable = !kicker_Params.balance_enable;
		dbgu_printf("%1d ", kicker_Params.balance_enable);
		break;
	case BALANCE_RL_P_ROW:
#ifdef KICKER_USE_BALANCE
		kicker_Params.balance_rl_p_gain += (float)val / 100.f;
		dbgu_printf("%1.3f    ", kicker_Params.balance_rl_p_gain);
#else
		kicker_Params.balance_rl_p_gain += (float)val;
		dbgu_printf("%3.1f    ", kicker_Params.balance_rl_p_gain);
#endif
		break;
	case BALANCE_RL_I_ROW:
#ifdef KICKER_USE_BALANCE
		kicker_Params.balance_rl_i_gain += (float)val / 100.f;
		dbgu_printf("%1.3f    ", kicker_Params.balance_rl_i_gain);
#else
		kicker_Params.balance_rl_i_gain += (float)val;
		dbgu_printf("%3.1f    ", kicker_Params.balance_rl_i_gain);
#endif
		break;
	case BALANCE_RL_D_ROW:
#ifdef KICKER_USE_BALANCE
		kicker_Params.balance_rl_d_gain += (float)val / 100.f;
		dbgu_printf("%1.3f    ", kicker_Params.balance_rl_d_gain);
#else
		kicker_Params.balance_rl_d_gain += (float)val;
		dbgu_printf("%3.1f    ", kicker_Params.balance_rl_d_gain);
#endif
		break;
	case BALANCE_FB_P_ROW:
#ifdef KICKER_USE_BALANCE
		kicker_Params.balance_fb_p_gain += (float)val / 100.f;
		dbgu_printf("%1.3f    ", kicker_Params.balance_fb_p_gain);
#else
		kicker_Params.balance_fb_p_gain += (float)val;
		dbgu_printf("%3.1f    ", kicker_Params.balance_fb_p_gain);
#endif
		break;
	case BALANCE_FB_I_ROW:
#ifdef KICKER_USE_BALANCE
		kicker_Params.balance_fb_i_gain += (float)val / 100.f;
		dbgu_printf("%1.3f ", kicker_Params.balance_fb_i_gain);
#else
		kicker_Params.balance_fb_i_gain += (float)val;
		dbgu_printf("%3.1f    ", kicker_Params.balance_fb_i_gain);
#endif
		break;
	case BALANCE_FB_D_ROW:
#ifdef KICKER_USE_BALANCE
		kicker_Params.balance_fb_d_gain += (float)val / 100.f;
		dbgu_printf("%1.3f    ", kicker_Params.balance_fb_d_gain);
#else
		kicker_Params.balance_fb_d_gain += (float)val;
		dbgu_printf("%3.1f    ", kicker_Params.balance_fb_d_gain);
#endif
		break;
	case SERVO_P_ROW:
		kicker_ServoParams.servo_gain_p += (float)val / 10.f;
		dbgu_printf("%2.2f    ", kicker_ServoParams.servo_gain_p);
		break;
	case SERVO_I_ROW:
		kicker_ServoParams.servo_gain_i += (float)val / 10.f;
		dbgu_printf("%2.2f    ", kicker_ServoParams.servo_gain_i);
		break;
	case SERVO_D_ROW:
		kicker_ServoParams.servo_gain_d += (float)val / 10.f;
		dbgu_printf("%2.2f    ", kicker_ServoParams.servo_gain_d);
		break;
	case BALANCE_L_Y_ROW:
		Kicker_AddBalanceLeftOffset( val);
		dbgu_printf("%d    ", Kicker_GetBalanceLeftOffset());
		break;
	case BALANCE_R_Y_ROW:
		Kicker_AddBalanceRightOffset( val);
		dbgu_printf("%d    ", Kicker_GetBalanceRightOffset());
		break;
	case OFFSET_L_HIP_PITCH_ROW:
		kicker_OffsetParams.hip_pitch_offset_l += val;
		dbgu_printf("%d    ", kicker_OffsetParams.hip_pitch_offset_l);
		break;
	case OFFSET_L_HIP_ROLL_ROW:
		kicker_OffsetParams.hip_roll_offset_l += val;
		dbgu_printf("%d    ", kicker_OffsetParams.hip_roll_offset_l);
		break;
	case OFFSET_L_ANKLE_PITCH_ROW:
		kicker_OffsetParams.ankle_pitch_offset_l += val;
		dbgu_printf("%d    ", kicker_OffsetParams.ankle_pitch_offset_l);
		break;
	case OFFSET_L_ANKLE_ROLL_ROW:
		kicker_OffsetParams.ankle_roll_offset_l += val;
		dbgu_printf("%d    ", kicker_OffsetParams.ankle_roll_offset_l);
		break;
	case OFFSET_R_HIP_PITCH_ROW:
		kicker_OffsetParams.hip_pitch_offset_r += val;
		dbgu_printf("%d    ", kicker_OffsetParams.hip_pitch_offset_r);
		break;
	case OFFSET_R_HIP_ROLL_ROW:
		kicker_OffsetParams.hip_roll_offset_r += val;
		dbgu_printf("%d    ", kicker_OffsetParams.hip_roll_offset_r);
		break;
	case OFFSET_R_ANKLE_PITCH_ROW:
		kicker_OffsetParams.ankle_pitch_offset_r += val;
		dbgu_printf("%d    ", kicker_OffsetParams.ankle_pitch_offset_r);
		break;
	case OFFSET_R_ANKLE_ROLL_ROW:
		kicker_OffsetParams.ankle_roll_offset_r += val;
		dbgu_printf("%d    ", kicker_OffsetParams.ankle_roll_offset_r);
		break;
	case RANGE_MULTIPLIER_LEG_ROW:
		kicker_RangeMultiplierLeg += (float)val / 1000.f;
		dbgu_printf("%1.3f   ", kicker_RangeMultiplierLeg);
		break;
	case RANGE_MULTIPLIER_HIP_ROW:
		kicker_RangeMultiplierHip += (float)val / 1000.f;
		dbgu_printf("%1.3f   ", kicker_RangeMultiplierHip);
		break;
	}
}

void Kickertuner_KickLeft() {
	//dbgu_GoToCursor( PARAM_COL, RESULT_ROW);
	//dbgu_printf("     ");
	struct vect3d ball;
	ball.x = 100;
	ball.y = -70;
	ball.z = 40;
	Kicker_KickForward( &ball, KICKER_TYPE_MEDIUM);
}

void Kickertuner_KickRight() {
	//dbgu_GoToCursor( PARAM_COL, RESULT_ROW);
	//dbgu_printf("     ");
	struct vect3d ball;
	ball.x = 100;
	ball.y = 70;
	ball.z = 40;
	Kicker_KickForward( &ball, KICKER_TYPE_MEDIUM);
}

void Kickertuner_DrawScreen(void) {
	int oldRow = Kickertuner_Row;
	dbgu_ClearScreen();
	Kickertuner_GoToCursor(0, 0);

	//           01234567890123456789012345678901234
	dbgu_printf("      == Kicker-Tuner ==         \r\n"); // 0
	dbgu_printf("Balance enable                   \r\n"); // 1
	dbgu_printf("L Balance y                      \r\n"); //
	dbgu_printf("R Balance y                      \r\n"); // 3
#ifdef KICKER_USE_BALANCE
	dbgu_printf("Balance RL P gain                \r\n"); // 5
	dbgu_printf("Balance RL I gain                \r\n"); //
	dbgu_printf("Balance RL D gain                \r\n"); // 7
	dbgu_printf("Balance FB P gain                \r\n"); //
	dbgu_printf("Balance FB I gain                \r\n"); // 9
	dbgu_printf("Balance FB D gain                \r\n"); //
#else
	dbgu_printf("L Init-Position x                \r\n"); // 5
	dbgu_printf("L Init-Position y                \r\n"); //
	dbgu_printf("L Init-Position z                \r\n"); // 7
	dbgu_printf("R Init-Position x                \r\n"); //
	dbgu_printf("R Init-Position y                \r\n"); // 9
	dbgu_printf("R Init-Position z                \r\n"); //
#endif
	dbgu_printf("L Offset hip pitch               \r\n"); // 11
	dbgu_printf("L Offset hip roll                \r\n"); //
	dbgu_printf("L Offset ankle pitch             \r\n"); // 13
	dbgu_printf("L Offset ankle roll              \r\n"); //
	dbgu_printf("R Offset hip pitch               \r\n"); // 15
	dbgu_printf("R Offset hip roll                \r\n"); //
	dbgu_printf("R Offset ankle pitch             \r\n"); // 17
	dbgu_printf("R Offset ankle roll              \r\n"); //
	dbgu_printf("Range multiplier leg             \r\n"); // 19
	dbgu_printf("Range multiplier hip             \r\n"); //
	dbgu_printf("Servo P gain                     \r\n"); // 21
	dbgu_printf("Servo I gain                     \r\n"); //
	dbgu_printf("Servo D gain                     \r\n"); // 23
	dbgu_printf("                                 \r\n"); //

	Kickertuner_PrintBalance();
	Kickertuner_PrintOffsets();
	Kickertuner_PrintRange();
	Kickertuner_PrintServo();

	Kickertuner_GoToCursor(USER_COL, oldRow);
    dbgu_printf("*");

}

unsigned char Kickertuner_Exit(void)
{
	Kickertuner_PrintCmd("Are you sure? (y/n)");
	if(dbgu_get_char() == 'y' || dbgu_get_char() == 'Y') {
		Kickertuner_PrintCmd("Exit Kickertest");
		return 1;
	}
	return 0;
}

unsigned char Kickertuner_CommandMode(int ch)
{
	char input[128] = {0,};
	char strParam[20][30];
	char cmd[80];
	char *token;
	int input_len;
	int idx = 0;

	Kickertuner_Old_Col = Kickertuner_Col;
	Kickertuner_Old_Row = Kickertuner_Row;
	Kickertuner_BeginCmd();

	while(1) {
		if( ch == 0x0D || ch == 0x0A) { // Return / Enter
			break;
		} else if (ch == 0x08 || ch == 0x7F) { // Backspace / Delete
			if(idx > 0) {
				ch = 0x08;
				dbgu_printf("%c", ch);
				ch = ' ';
				dbgu_printf("%c", ch);
				ch = 0x08;
				dbgu_printf("%c", ch);
				input[--idx] = 0;
			}
		} else if( ch >= 'A' && ch <= 'z') {
			if(idx < 127) {
				dbgu_printf("%c", ch);
				input[idx++] = (char)ch;
			}
		}
		ch = dbgu_get_char();
	}

	//fflush(stdin);
	input_len = strlen(input);
	if(input_len > 0) {
		token = strtok( input, " ");
		if(token != 0) {
			strcpy( cmd, token);
			token = strtok( 0, " ");
			int num_param = 0;
			while(token != 0) {
				strcpy(strParam[num_param++], token);
				token = strtok( 0, " ");
			}

			if(strcmp(cmd, "exit") == 0) {
				return !Kickertuner_Exit();
			} else if(strcmp(cmd, "save") == 0) {
				Kicker_SaveParams();
			} else if(strcmp(cmd, "load") == 0) {
				Kicker_LoadParams();
				Kickertuner_DrawScreen();
			} else if(strcmp(cmd, "reset") == 0) {
				//Kicker_ResetParameters();
				//Kickertuner_ResetAll();
				Kickertuner_DrawScreen();
			} else {
				Kickertuner_PrintCmd("Bad command!");
			}
		}
	}

	Kickertuner_GoToCursor( Kickertuner_Old_Col, Kickertuner_Old_Row);
	return 1;
}

void Kickertuner_Main(void) {
	//Kickertuner_ResetAll();
	Kickertuner_Col = USER_COL;
	Kickertuner_Row = HEADING_ROW + 1;
	Kickertuner_Old_Col = Kickertuner_Col;
	Kickertuner_Old_Row = Kickertuner_Row;

	Kickertuner_DrawScreen();

    while(1) {
		unsigned long loopStartTime = TC_GetSystemTicks();

		UI_ToggleLED(0);

		if( !Kicker_IsRunning()) {
			Kicker_Balance();
		}

		if (dbgu_char_available() == 1) {
			int ch = dbgu_get_char();
			if(ch == 0xe0 || ch == 0x1b) {
				if (ch == 0x1b) {
					ch = dbgu_get_char();
				}
				ch = dbgu_get_char();
				if(ch == 0x48) { 		// Up arrow key
					Kickertuner_MoveCursorUp();
				} else if(ch == 0x50) { // Down arrow key
					Kickertuner_MoveCursorDown();
				}
			} else if( ch == '-' ) {
				Kickertuner_Add(-1);
			} else if( ch == '+' ) {
				Kickertuner_Add(1);
			} else if( ch == '_' ) {
				Kickertuner_Add(-10);
			} else if( ch == '*' ) {
				Kickertuner_Add(10);
			} else if( ch == '.') {
				Kickertuner_MoveCursorDown();
			} else if( ch == ',') {
				Kickertuner_MoveCursorUp();
			} else if( ch == ':') {
				Kickertuner_MoveCursorToRow( HEADING_ROW + 1);
			} else if( ch == ';') {
				Kickertuner_MoveCursorToRow( CMD_ROW - 1);
			} else if( ch == 'x') {
				Kickertuner_DrawScreen();
			} else if( ch == '1') {
				Kickertuner_KickLeft();
			} else if( ch == '2') {
				Kickertuner_KickRight();
			} else if( ch == '0') {
				Kicker_CancelKick();
			} else {
				if( !Kickertuner_CommandMode( ch)) {
					break;
				}
			}

		}

		// waste remaining loop time
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION);
    }
    dbgu_ClearScreen();
}

