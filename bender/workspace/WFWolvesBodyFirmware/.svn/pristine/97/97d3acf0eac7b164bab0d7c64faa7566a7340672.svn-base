/*
 * kinematictest.c
 *
 *  Created on: 08.11.2012
 *      Author: Oliver
 */

#include "dbgu/dbgu.h"
#include "tc/tc.h"
#include "ui/ui.h"
#include "servo/servo.h"
#include "imu/imu.h"
#include "kinematics/inverse.h"
#include "eeprom/eeprom.h"
#include "kicker/kicker.h"
#include "kicker/balance.h"
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif

#ifdef DEBUG_X86
#define MAIN_LOOP_DURATION			24 // ms
#else
#define MAIN_LOOP_DURATION			20//8 // ms
#endif

#ifndef SCREEN_COL
#define SCREEN_COL		35
//#define CMD_COL			2
//#define PARAM_COL		27
#endif


// Position of Row
enum {
    LEG_LEFT_ROW  = 2,
    LEG_RIGHT_ROW = 3,
    ARM_LEFT_ROW  = 4,
    ARM_RIGHT_ROW = 5,
    PID1_ROW 	  = 6,
    PID2_ROW 	  = 7,
    BALANCED_ROW  = 9,
};

#define SCREEN_ROW 8

enum {
	NAME_COL	= 0,
	VEC_X_COL	= 11,
	VEC_Y_COL	= 16,
	VEC_Z_COL	= 21,
	YAW_COL		= 26,
	USER_COL	= 31
};

int Kinematictest_Col = 0;
int Kinematictest_Row = 0;
int Kinematictest_Old_Col;
int Kinematictest_Old_Row;

float Kinematictest_GainP1 = 0.0f;
float Kinematictest_GainI1 = 0.0f;
float Kinematictest_GainD1 = 0.0f;
float Kinematictest_GainP2 = 0.0f;
float Kinematictest_GainI2 = 0.0f;
float Kinematictest_GainD2 = 0.0f;

unsigned char Kinematictest_balancedLeft = 0;
unsigned char Kinematictest_balancedRight = 0;

struct vect3d_ext extremity[COUNT_EXTREMITIES];

void Kinematictest_CalcInverse();

void Kinematictest_ResetArmLeft() {
	extremity[ARM_LEFT].x = ARM_VEC_X_INIT;
	extremity[ARM_LEFT].y = ARM_VEC_Y_INIT;
	extremity[ARM_LEFT].z = ARM_VEC_Z_INIT;
	extremity[ARM_LEFT].yaw = 0;
}

void Kinematictest_ResetArmRight() {
	extremity[ARM_RIGHT].x = ARM_VEC_X_INIT;
	extremity[ARM_RIGHT].y = ARM_VEC_Y_INIT;
	extremity[ARM_RIGHT].z = ARM_VEC_Z_INIT;
	extremity[ARM_RIGHT].yaw = 0;
}

void Kinematictest_ResetLegLeft() {
	extremity[LEG_LEFT].x = LEG_VEC_X_INIT;
	extremity[LEG_LEFT].y = LEG_VEC_Y_INIT;
	extremity[LEG_LEFT].z = LEG_VEC_Z_INIT;
	extremity[LEG_LEFT].yaw = 0;
}

void Kinematictest_ResetLegRight() {
	extremity[LEG_RIGHT].x = LEG_VEC_X_INIT;
	extremity[LEG_RIGHT].y = LEG_VEC_Y_INIT;
	extremity[LEG_RIGHT].z = LEG_VEC_Z_INIT;
	extremity[LEG_RIGHT].yaw = 0;
}

void Kinematictest_ResetAll() {
	Kinematictest_GainP1 = 0.45f;
	Kinematictest_GainI1 = 0.03f;
	Kinematictest_GainD1 = 0.01f;
	Kinematictest_GainP2 = 0.45f;
	Kinematictest_GainI2 = 0.03f;
	Kinematictest_GainD2 = 0.01f;
	Kinematictest_balancedLeft = 0;
	Kinematictest_balancedRight = 0;
	Kinematictest_ResetArmLeft();
	Kinematictest_ResetArmRight();
	Kinematictest_ResetLegLeft();
	Kinematictest_ResetLegRight();
	Kinematictest_CalcInverse();
}

void Kinematictest_ResetActual() {
	switch( Kinematictest_Row) {
	case ARM_LEFT_ROW:
		Kinematictest_ResetArmLeft();
		break;
	case ARM_RIGHT_ROW:
		Kinematictest_ResetArmRight();
		break;
	case LEG_LEFT_ROW:
		Kinematictest_ResetLegLeft();
		break;
	case LEG_RIGHT_ROW:
		Kinematictest_ResetLegRight();
		break;
	}
	Kinematictest_CalcInverse();
}

void Kinematictest_PrintPID() {
	dbgu_GoToCursor( VEC_X_COL, PID1_ROW);
	dbgu_printf("%1.2f %1.2f %1.2f ", Kinematictest_GainP1, Kinematictest_GainI1, Kinematictest_GainD1);
	dbgu_GoToCursor( VEC_X_COL, PID2_ROW);
	dbgu_printf("%1.2f %1.2f %1.2f ", Kinematictest_GainP2, Kinematictest_GainI2, Kinematictest_GainD2);
}

void Kinematictest_GoToCursor(int col, int row) {
	dbgu_GoToCursor( col, row);
	Kinematictest_Col = col;
	Kinematictest_Row = row;
}

void Kinematictest_MoveCursorUp() {
	if(Kinematictest_Row > LEG_LEFT_ROW) {
		Kinematictest_GoToCursor(USER_COL, Kinematictest_Row);
		dbgu_printf(" ");
		Kinematictest_GoToCursor(USER_COL, Kinematictest_Row-1);
		dbgu_printf("*");
	}
}

void Kinematictest_MoveCursorDown() {
	if(Kinematictest_Row + 1 < SCREEN_ROW) {
		Kinematictest_GoToCursor(USER_COL, Kinematictest_Row);
		dbgu_printf(" ");
		Kinematictest_GoToCursor(USER_COL, Kinematictest_Row+1);
		dbgu_printf("*");
	}
}

void Kinematictest_AddYaw( signed short val) {
	switch( Kinematictest_Row) {
	case ARM_LEFT_ROW:
		extremity[ARM_LEFT].yaw += val;
		break;
	case ARM_RIGHT_ROW:
		extremity[ARM_RIGHT].yaw += val;
		break;
	case LEG_LEFT_ROW:
		extremity[LEG_LEFT].yaw += val;
		break;
	case LEG_RIGHT_ROW:
		extremity[LEG_RIGHT].yaw += val;
		break;
	default:
		return;
	}
	Kinematictest_CalcInverse();
}

void Kinematictest_AddVectorX( signed short val) {
	switch( Kinematictest_Row) {
	case ARM_LEFT_ROW:
		extremity[ARM_LEFT].x += val;
		break;
	case ARM_RIGHT_ROW:
		extremity[ARM_RIGHT].x += val;
		break;
	case LEG_LEFT_ROW:
		extremity[LEG_LEFT].x += val;
		break;
	case LEG_RIGHT_ROW:
		extremity[LEG_RIGHT].x += val;
		break;
	case PID1_ROW:
		Kinematictest_GainP1 += (float)val / 100.f;
		Kinematictest_PrintPID();
		return;
	case PID2_ROW:
		Kinematictest_GainP2 += (float)val / 100.f;
		Kinematictest_PrintPID();
		return;
	default:
		return;
	}
	Kinematictest_CalcInverse();
}

void Kinematictest_AddVectorY( signed short val) {
	switch( Kinematictest_Row) {
	case ARM_LEFT_ROW:
		extremity[ARM_LEFT].y += val;
		break;
	case ARM_RIGHT_ROW:
		extremity[ARM_RIGHT].y += val;
		break;
	case LEG_LEFT_ROW:
		extremity[LEG_LEFT].y += val;
		break;
	case LEG_RIGHT_ROW:
		extremity[LEG_RIGHT].y += val;
		break;
	case PID1_ROW:
		Kinematictest_GainI1 += (float)val / 100.f;
		Kinematictest_PrintPID();
		return;
	case PID2_ROW:
		Kinematictest_GainI2 += (float)val / 100.f;
		Kinematictest_PrintPID();
		return;
	default:
		return;
	}
	Kinematictest_CalcInverse();
}

void Kinematictest_AddVectorZ( signed short val) {
	switch( Kinematictest_Row) {
	case ARM_LEFT_ROW:
		extremity[ARM_LEFT].z += val;
		break;
	case ARM_RIGHT_ROW:
		extremity[ARM_RIGHT].z += val;
		break;
	case LEG_LEFT_ROW:
		extremity[LEG_LEFT].z += val;
		break;
	case LEG_RIGHT_ROW:
		extremity[LEG_RIGHT].z += val;
		break;
	case PID1_ROW:
		Kinematictest_GainD1 += (float)val / 100.f;
		Kinematictest_PrintPID();
		return;
	case PID2_ROW:
		Kinematictest_GainD2 += (float)val / 100.f;
		Kinematictest_PrintPID();
		return;default:
		return;
	}
	Kinematictest_CalcInverse();
}

void Kinematictest_PrintExtremity(struct vect3d_ext ext, int row) {
	dbgu_GoToCursor( VEC_X_COL, row);
	dbgu_printf("%4d %4d %4d %4d ", ext.x, ext.y, ext.z, ext.yaw);
}

void Kinematictest_PrintBalanced() {
	dbgu_GoToCursor( YAW_COL, BALANCED_ROW);
	dbgu_printf("%2d, %2d ", Kinematictest_balancedLeft, Kinematictest_balancedRight);
}

void Kinematictest_DrawScreen(void) {
	int oldRow = Kinematictest_Row;
	dbgu_ClearScreen();
	Kinematictest_GoToCursor(0, 0);

	//           01234567890123456789012345678901234
	dbgu_printf("      == Kinematic-Test ==         \r\n"); // 0
	dbgu_printf("            X    Y    Z   Yaw      \r\n"); //
	dbgu_printf("LEG_LEFT                           \r\n"); // 2
	dbgu_printf("LEG_RIGHT                          \r\n"); //
	dbgu_printf("ARM_LEFT                           \r\n"); // 4
	dbgu_printf("ARM_RIGHT                          \r\n"); //
	dbgu_printf(" PID rl                            \r\n"); // 6
	dbgu_printf(" PID fb                            \r\n"); // 6
	dbgu_printf(" Error                             \r\n"); //
	dbgu_printf(" balanced (left, right) =          \r\n"); // 8
	dbgu_printf("                                   \r\n"); //
	dbgu_printf("[n] Toggle balance mode left       \r\n"); //
	dbgu_printf("[m] Toggle balance mode right      \r\n"); //
	dbgu_printf("[k] Reset actual                   \r\n"); //
	dbgu_printf("[i] Reset all                      \r\n"); //
	dbgu_printf("[x] Draw screen                    \r\n"); //
	dbgu_printf("                                   \r\n"); //
	dbgu_printf("      X   Y   Z  Yaw               \r\n"); //
	dbgu_printf(" +   [Q] [W] [E] [R] [T]           \r\n"); //
	dbgu_printf(" -    [A] [S] [D] [F] [G]          \r\n"); //

	for( int i = 0; i < COUNT_EXTREMITIES; i++) {
		Kinematictest_PrintExtremity( extremity[i], i + LEG_LEFT_ROW); // LEG_LEFT_ROW as first Extremity-Row
	}
	Kinematictest_PrintPID();
	Kinematictest_PrintBalanced();

	Kinematictest_GoToCursor(USER_COL, oldRow);
    dbgu_printf("*");

}

void Kinematictest_CalcInverse() {
	inverse_startCycle();
	for( int i = 0; i < COUNT_EXTREMITIES; i++) {
		inverse_calc(i, &extremity[i]);
		Kinematictest_PrintExtremity( extremity[i], i + LEG_LEFT_ROW); // LEG_LEFT_ROW as first Extremity-Row
	}
	inverse_endCycle(0);
}

void KinematicTest_Main(void) {
	Kinematictest_ResetAll();
	Kinematictest_Col = USER_COL;
	Kinematictest_Row = LEG_LEFT_ROW;
	Kinematictest_Old_Col = Kinematictest_Col;
	Kinematictest_Old_Row = Kinematictest_Row;

	Kinematictest_DrawScreen();

    while(1) {
		unsigned long loopStartTime = TC_GetSystemTicks();

		UI_ToggleLED(0);

		if( Kinematictest_balancedRight || Kinematictest_balancedLeft) {
			IMU_Update();
			inverse_startCycle();
			inverse_calc( ARM_RIGHT, &extremity[ARM_RIGHT]);
			inverse_calc( ARM_LEFT, &extremity[ARM_LEFT]);
			if( Kinematictest_balancedRight) {
				//inverse_calc_balanced_gain(LEG_RIGHT, &extremity[LEG_RIGHT], Kinematictest_balancedGain);
				inverse_calc( LEG_RIGHT, &extremity[LEG_RIGHT]);
				balance_right_rl( Kinematictest_GainP1, Kinematictest_GainI1, Kinematictest_GainD1, 0.02f);
				balance_right_fb( Kinematictest_GainP2, Kinematictest_GainI2, Kinematictest_GainD2, 0.02f);
			}
			if( Kinematictest_balancedLeft) {
				int e1, e2;
				//inverse_calc_balanced_gain(LEG_LEFT, &extremity[LEG_LEFT], Kinematictest_balancedGain);
				inverse_calc( LEG_LEFT, &extremity[LEG_LEFT]);
				e1 = balance_left_rl( Kinematictest_GainP1, Kinematictest_GainI1, Kinematictest_GainD1, 0.02f);
				e2 = balance_left_fb( Kinematictest_GainP2, Kinematictest_GainI2, Kinematictest_GainD2, 0.02f);
				dbgu_GoToCursor( VEC_X_COL, BALANCED_ROW - 1);
				dbgu_printf("%7d %7d ", e1, e2);
			}
			inverse_endCycle(0);
		}

		if (dbgu_char_available() == 1) {
			int ch = dbgu_get_char();
			if(ch == 0xe0 || ch == 0x1b) {
				if (ch == 0x1b) {
					ch = dbgu_get_char();
				}
				ch = dbgu_get_char();
				if(ch == 0x48) { 	// Up arrow key
					Kinematictest_MoveCursorUp();
				} else if(ch == 0x50) { // Down arrow key
					Kinematictest_MoveCursorDown();
				}
			} else if( ch == 'a' ) {
				Kinematictest_AddVectorX(-1);
			} else if( ch == 'q' ) {
				Kinematictest_AddVectorX(1);
			} else if( ch == 's' ) {
				Kinematictest_AddVectorY(-1);
			} else if( ch == 'w' ) {
				Kinematictest_AddVectorY(1);
			} else if( ch == 'd' ) {
				Kinematictest_AddVectorZ(-1);
			} else if( ch == 'e' ) {
				Kinematictest_AddVectorZ(1);
			} else if( ch == 'f' ) {
				Kinematictest_AddYaw(-1);
			} else if( ch == 'r' ) {
				Kinematictest_AddYaw(1);
			} else if( ch == 'g') {
				Kinematictest_MoveCursorDown();
			} else if( ch == 't') {
				Kinematictest_MoveCursorUp();
			} else if( ch == 'x') {
				Kinematictest_DrawScreen();
			} else if( ch == 'k') {
				Kinematictest_ResetActual();
				Kinematictest_DrawScreen();
			} else if( ch == 'i') {
				Kinematictest_ResetAll();
				Kinematictest_DrawScreen();
			} else if( ch == 'n') {
				Kinematictest_balancedLeft = !Kinematictest_balancedLeft;
				Kinematictest_PrintBalanced();
			} else if( ch == 'm') {
				Kinematictest_balancedRight = !Kinematictest_balancedRight;
				Kinematictest_PrintBalanced();
			} else if( ch == 3) { // Ctrl + C
				break;
			}

		}

		// waste remaining loop time
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION);
    }
    dbgu_ClearScreen();
}
