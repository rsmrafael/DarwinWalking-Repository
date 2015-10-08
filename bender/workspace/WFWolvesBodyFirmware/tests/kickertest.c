/*
 * kickertest.c
 *
 *  Created on: 28.11.2012
 *      Author: Oliver
 */

#include "kickertest.h"
#include "dbgu/dbgu.h"
#include "tc/tc.h"
#include "ui/ui.h"
#include "servo/servo.h"
#include "imu/imu.h"
#include "kinematics/inverse.h"
#include "eeprom/eeprom.h"
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

// elements of Kickertest
enum {
	KICKERTEST_BALL,
	KICKERTEST_TARGET,
	KICKERTEST_COUNT
};

// rows of output
enum {
    HEADING_ROW,				// 0
    BALL_X_ROW,
    BALL_Y_ROW,					// 2
    BALL_Z_ROW,
    TARGET_X_ROW,				// 4
    TARGET_Y_ROW,
    TARGET_Z_ROW,				// 6
    KICKTYPE_ROW,
    FADE_ENABLE_ROW,
    CMD_ROW,
};

// columns of output
enum {
	NAME_COL	= 0,
	PARAM_COL	= 16,
	USER_COL	= 33
};

int Kickertest_Col = 0;
int Kickertest_Row = 0;
int Kickertest_Old_Col;
int Kickertest_Old_Row;

extern unsigned char kicker_fadeEnable;
struct vect3d Kickertest_element[KICKERTEST_COUNT];
enum kicker_type kickertest_type = KICKER_TYPE_MEDIUM;

void Kickertest_ResetBall() {
	Kickertest_element[KICKERTEST_BALL].x = 100;
	Kickertest_element[KICKERTEST_BALL].y = 70;
	Kickertest_element[KICKERTEST_BALL].z = 40;
}

void Kickertest_ResetTarget() {
	Kickertest_element[KICKERTEST_TARGET].x = 100;
	Kickertest_element[KICKERTEST_TARGET].y = 45;
	Kickertest_element[KICKERTEST_TARGET].z = 5;
}

void Kickertest_ResetAll() {
	Kickertest_ResetBall();
	Kickertest_ResetTarget();
}

void Kickertest_GoToCursor(int col, int row) {
	dbgu_GoToCursor( col, row);
	Kickertest_Col = col;
	Kickertest_Row = row;
}

void Kickertest_BeginCmd(void) {
	dbgu_GoToCursor(0, CMD_ROW);
	dbgu_printf("] ");
	for(int i = 0; i < (SCREEN_COL - 2); i++)
		dbgu_printf(" ");

	dbgu_GoToCursor(3, CMD_ROW);
}

void Kickertest_PrintCmd(const char *message) {
	Kickertest_BeginCmd();
	dbgu_printf("%s", message);
}

void Kickertest_MoveCursorToRow( int row) {
	if( row >= BALL_X_ROW && row <= CMD_ROW - 1) {
		Kickertest_GoToCursor(USER_COL, Kickertest_Row);
		dbgu_printf(" ");
		Kickertest_Row = row;
		Kickertest_GoToCursor(USER_COL, Kickertest_Row);
		dbgu_printf("*");
	}
}

void Kickertest_MoveCursorUp() {
	if( Kickertest_Row > BALL_X_ROW) {
		Kickertest_GoToCursor(USER_COL, Kickertest_Row);
		dbgu_printf(" ");
		Kickertest_GoToCursor(USER_COL, Kickertest_Row-1);
		dbgu_printf("*");
	}
}

void Kickertest_MoveCursorDown() {
	if(Kickertest_Row < CMD_ROW - 1) {
		Kickertest_GoToCursor(USER_COL, Kickertest_Row);
		dbgu_printf(" ");
		Kickertest_GoToCursor(USER_COL, Kickertest_Row+1);
		dbgu_printf("*");
	}
}

void Kickertest_PrintElement(struct vect3d* ext, int row) {
	dbgu_GoToCursor( PARAM_COL, row);
	dbgu_printf("%4d ", ext->x);
	dbgu_GoToCursor( PARAM_COL, row + 1);
	dbgu_printf("%4d ", ext->y);
	dbgu_GoToCursor( PARAM_COL, row + 2);
	dbgu_printf("%4d ", ext->z);
}

void Kickertest_PrintKicktype() {
	dbgu_GoToCursor( PARAM_COL, KICKTYPE_ROW);
	dbgu_printf("%d ", kickertest_type);
	switch( kickertest_type) {
	case KICKER_TYPE_MINIMAL:
		dbgu_printf("(MINIMAL) ");
		break;
	case KICKER_TYPE_SHORT:
		dbgu_printf("(SHORT)   ");
		break;
	case KICKER_TYPE_MEDIUM:
		dbgu_printf("(MEDIUM)  ");
		break;
	case KICKER_TYPE_POWER:
		dbgu_printf("(POWER)   ");
		break;
	case KICKER_TYPE_MAXIMAL:
		dbgu_printf("(MAXIMAL) ");
		break;
	}
}

void Kickertest_Add( signed short val) {
	dbgu_GoToCursor( PARAM_COL, Kickertest_Row);
	switch( Kickertest_Row) {
	case BALL_X_ROW:
		Kickertest_element[KICKERTEST_BALL].x += val;
		dbgu_printf("%4d ", Kickertest_element[KICKERTEST_BALL].x);
		break;
	case BALL_Y_ROW:
		Kickertest_element[KICKERTEST_BALL].y += val;
		dbgu_printf("%4d ", Kickertest_element[KICKERTEST_BALL].y);
		break;
	case BALL_Z_ROW:
		Kickertest_element[KICKERTEST_BALL].z += val;
		dbgu_printf("%4d ", Kickertest_element[KICKERTEST_BALL].z);
		break;
	case TARGET_X_ROW:
		Kickertest_element[KICKERTEST_TARGET].x += val;
		dbgu_printf("%4d ", Kickertest_element[KICKERTEST_TARGET].x);
		break;
	case TARGET_Y_ROW:
		Kickertest_element[KICKERTEST_TARGET].y += val;
		dbgu_printf("%4d ", Kickertest_element[KICKERTEST_TARGET].y);
		break;
	case TARGET_Z_ROW:
		Kickertest_element[KICKERTEST_TARGET].z += val;
		dbgu_printf("%4d ", Kickertest_element[KICKERTEST_TARGET].z);
		break;
	case KICKTYPE_ROW:
		kickertest_type = MID(KICKER_TYPE_MINIMAL, kickertest_type + val, KICKER_TYPE_MAXIMAL);
		Kickertest_PrintKicktype();
		break;
	case FADE_ENABLE_ROW:
		kicker_fadeEnable = !kicker_fadeEnable;
		dbgu_printf("%d ", kicker_fadeEnable);
		break;
	}
}

void Kickertest_Kick() {
	//dbgu_GoToCursor( PARAM_COL, RESULT_ROW);
	//dbgu_printf("     ");
	Kicker_Kick( &Kickertest_element[KICKERTEST_BALL]);
}

void Kickertest_KickForward() {
	//dbgu_GoToCursor( PARAM_COL, RESULT_ROW);
	//dbgu_printf("     ");
	Kicker_KickForward( &Kickertest_element[KICKERTEST_BALL], kickertest_type);
}

void Kickertest_KickBackward() {
	//dbgu_GoToCursor( PARAM_COL, RESULT_ROW);
	//dbgu_printf("     ");
	Kicker_KickBackward( &Kickertest_element[KICKERTEST_BALL], kickertest_type);
}

void Kickertest_KickSideward() {
	//dbgu_GoToCursor( PARAM_COL, RESULT_ROW);
	//dbgu_printf("     ");
	Kicker_KickSideward( &Kickertest_element[KICKERTEST_BALL], kickertest_type);
}

void Kickertest_KickToTarget() {
	//dbgu_GoToCursor( PARAM_COL, RESULT_ROW);
	//dbgu_printf("     ");
	Kicker_KickToTarget( &Kickertest_element[KICKERTEST_BALL], &Kickertest_element[KICKERTEST_TARGET]);
}

void Kickertest_KickHigh() {
	//dbgu_GoToCursor( PARAM_COL, RESULT_ROW);
	Kicker_KickHigh( &Kickertest_element[KICKERTEST_BALL], &Kickertest_element[KICKERTEST_TARGET], kickertest_type);
}

void Kickertest_DrawScreen(void) {
	int oldRow = Kickertest_Row;
	dbgu_ClearScreen();
	Kickertest_GoToCursor(0, 0);

	//           01234567890123456789012345678901234
	dbgu_printf("       == Kicker-Test ==         \r\n"); // 0
	dbgu_printf("Ball x (mm)                      \r\n"); //
	dbgu_printf("Ball y (mm)                      \r\n"); // 2
	dbgu_printf("Ball z (mm)                      \r\n"); //
	dbgu_printf("Target x (mm)                    \r\n"); // 4
	dbgu_printf("Target y (mm)                    \r\n"); //
	dbgu_printf("Target z (mm)                    \r\n"); // 6
	dbgu_printf("Kicktype                         \r\n"); //
	dbgu_printf("Fade enable                      \r\n"); //
	dbgu_printf("                                 \r\n"); //
	dbgu_printf("                                 \r\n"); //
	dbgu_printf("[1] Kick Ball           (B)      \r\n"); //
	dbgu_printf("[2] Kick Ball Forward   (B,K)    \r\n"); //
	dbgu_printf("[3] Kick Ball Backward  (B,K)    \r\n"); //
	dbgu_printf("[4] Kick Ball Sideward  (B,K)    \r\n"); //
	dbgu_printf("[5] Kick Ball to Target (B,T)    \r\n"); //
	dbgu_printf("[6] High Kick (incorrect!)       \r\n"); //
	dbgu_printf("[0] Cancel Kick                  \r\n"); //
	dbgu_printf("                                 \r\n"); //

	for( int i = 0; i < KICKERTEST_COUNT; i++) {
		Kickertest_PrintElement( &Kickertest_element[i], 3*i + BALL_X_ROW); // BALL_ROW as first Element-Row
	}
	Kickertest_PrintKicktype();
	dbgu_GoToCursor( PARAM_COL, FADE_ENABLE_ROW);
	dbgu_printf("%d ", kicker_fadeEnable);
	Kickertest_GoToCursor(USER_COL, oldRow);
    dbgu_printf("*");

}

unsigned char Kickertest_Exit(void)
{
	Kickertest_PrintCmd("Are you sure? (y/n)");
	if(dbgu_get_char() == 'y' || dbgu_get_char() == 'Y') {
		Kickertest_PrintCmd("Exit Kickertest");
		return 1;
	}
	return 0;
}

unsigned char Kickertest_CommandMode(int ch)
{
	char input[128] = {0,};
	char strParam[20][30];
	char cmd[80];
	char *token;
	int input_len;
	int idx = 0;

	Kickertest_Old_Col = Kickertest_Col;
	Kickertest_Old_Row = Kickertest_Row;
	Kickertest_BeginCmd();

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
				return !Kickertest_Exit();
			} else if(strcmp(cmd, "reset") == 0) {
				//Kicker_ResetParameters();
				Kickertest_ResetAll();
				Kickertest_DrawScreen();
			} else {
				Kickertest_PrintCmd("Bad command!");
			}
		}
	}

	Kickertest_GoToCursor( Kickertest_Old_Col, Kickertest_Old_Row);
	return 1;
}

void Kickertest_Main(void) {
	Kickertest_ResetAll();
	Kickertest_Col = USER_COL;
	Kickertest_Row = BALL_X_ROW;
	Kickertest_Old_Col = Kickertest_Col;
	Kickertest_Old_Row = Kickertest_Row;

	Kickertest_DrawScreen();

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
					Kickertest_MoveCursorUp();
				} else if(ch == 0x50) { // Down arrow key
					Kickertest_MoveCursorDown();
				}
			} else if( ch == '-' ) {
				Kickertest_Add(-1);
			} else if( ch == '+' ) {
				Kickertest_Add(1);
			} else if( ch == '_' ) {
				Kickertest_Add(-10);
			} else if( ch == '*' ) {
				Kickertest_Add(10);
			} else if( ch == '.') {
				Kickertest_MoveCursorDown();
			} else if( ch == ',') {
				Kickertest_MoveCursorUp();
			} else if( ch == ':') {
				Kickertest_MoveCursorToRow( BALL_X_ROW);
			} else if( ch == ';') {
				Kickertest_MoveCursorToRow( CMD_ROW - 1);
			} else if( ch == 'x') {
				Kickertest_DrawScreen();
			} else if( ch == '1') {
				Kickertest_Kick();
			} else if( ch == '2') {
				Kickertest_KickForward();
			} else if( ch == '3') {
				Kickertest_KickBackward();
			} else if( ch == '4') {
				Kickertest_KickSideward();
			} else if( ch == '5') {
				Kickertest_KickToTarget();
			} else if( ch == '6') {
				Kickertest_KickHigh();
			} else if( ch == '0') {
				Kicker_CancelKick();
			} else {
				if( !Kickertest_CommandMode( ch)) {
					break;
				}
			}

		}

		// waste remaining loop time
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION);
    }
    dbgu_ClearScreen();
}

