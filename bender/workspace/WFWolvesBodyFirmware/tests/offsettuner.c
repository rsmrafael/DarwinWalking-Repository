/*
 * offsettuner.c
 *
 *  Created on: 02.04.2013
 *      Author: Stefan
 */

#include "dbgu/dbgu.h"
#include "servo/servo.h"
#include "eeprom/eeprom.h"
#include "tc/tc.h"
#include "ui/ui.h"
#include "movements/action.h"
#include "config/config.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#endif

#ifdef DEBUG_X86
#define MAIN_LOOP_DURATION			24 // ms
#else
#define MAIN_LOOP_DURATION			10 // ms
#endif

#define SCREEN_COL			80
#define SCREEN_ROW			22

// Position of Column
#define CMD_COL			2
#define GOAL_COL		21
#define OFFSET_COL		28
#define MODVAL_COL		35
#define PRSVAL_COL		42
#define ERRORS_COL		49
#define P_GAIN_COL      56
#define I_GAIN_COL      63
#define D_GAIN_COL      70

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
#define CMD_ROW		21

#define MOTION_ANGLE_TO_VAL4096(angle)	(lroundf(-(angle) * 11.378) + 2048) // 4096 / 360
#define MOTION_VAL4096_TO_ANGLE(value)	(-((value) - 2048) * 0.087890625) // 360 / 4096

int OffsetTuner_Col = OFFSET_COL;
int OffsetTuner_Row = ID_1_ROW;
int OffsetTuner_Old_Col;
int OffsetTuner_Old_Row;
unsigned char OffsetTuner_bBeginCommandMode = 0;
unsigned char OffsetTuner_bEdited = 0;
int OffsetTuner_indexPage = 1;
PAGE OffsetTuner_Page;
STEP OffsetTuner_Step;
STEP OffsetTuner_PresentPos;

int OffsetTuner_P_Gain[SERVO_COUNT_MAX];
int OffsetTuner_I_Gain[SERVO_COUNT_MAX];
int OffsetTuner_D_Gain[SERVO_COUNT_MAX];

//                           1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16    17    18    19    20
int InitPose[21] = { 2047, 1480, 2610, 1747, 2343, 2147, 1944, 2047, 2047, 2047, 2047, 2013, 2080, 2047, 2047, 2063, 2030, 2047, 2047, 2047, 2170 };

void OffsetTuner_DrawStep(int idx);
int OffsetTuner_GetValue();
void OffsetTuner_SetValue(int value);

void OffsetTuner_GoToCursor(int col, int row) {
#ifdef WIN32
	HANDLE screen_buffer_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(screen_buffer_handle, coord);
#else
	dbgu_printf("\x1B[%d;%dH", row + 1, col + 1);
#endif
	OffsetTuner_Col = col;
	OffsetTuner_Row = row;
}

void OffsetTuner_ClearScreen(void) {
#ifdef WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, 0 };
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
	OffsetTuner_GoToCursor(0, 0);
}

void OffsetTuner_PrintCmd(const char *message) {
	int len = strlen(message);
	OffsetTuner_GoToCursor(0, CMD_ROW);

	dbgu_printf("] %s", message);
	for (int i = 0; i < (SCREEN_COL - (len + 2)); i++)
		dbgu_printf(" ");

	OffsetTuner_GoToCursor(len + 2, CMD_ROW);
}

void OffsetTuner_ClearCmd() {
	OffsetTuner_PrintCmd("");
}

void OffsetTuner_MoveUpCursor() {
	if (OffsetTuner_Col >= GOAL_COL && OffsetTuner_Col <= D_GAIN_COL) {
		if (OffsetTuner_Row > ID_1_ROW)
			OffsetTuner_GoToCursor(OffsetTuner_Col, OffsetTuner_Row - 1);
	}
}

void OffsetTuner_MoveDownCursor() {
	if (OffsetTuner_Col >= GOAL_COL && OffsetTuner_Col <= D_GAIN_COL) {
		if (OffsetTuner_Row < ID_20_ROW)
			OffsetTuner_GoToCursor(OffsetTuner_Col, OffsetTuner_Row + 1);
	}
}

void OffsetTuner_MoveLeftCursor() {
	switch (OffsetTuner_Col) {
		case OFFSET_COL:
			OffsetTuner_GoToCursor(GOAL_COL, OffsetTuner_Row);
			break;

		case MODVAL_COL:
			OffsetTuner_GoToCursor(OFFSET_COL, OffsetTuner_Row);
			break;

		case PRSVAL_COL:
			OffsetTuner_GoToCursor(MODVAL_COL, OffsetTuner_Row);
			break;

		case ERRORS_COL:
			OffsetTuner_GoToCursor(PRSVAL_COL, OffsetTuner_Row);
			break;

		case P_GAIN_COL:
			OffsetTuner_GoToCursor(ERRORS_COL, OffsetTuner_Row);
			break;

		case I_GAIN_COL:
			OffsetTuner_GoToCursor(P_GAIN_COL, OffsetTuner_Row);
			break;

		case D_GAIN_COL:
			OffsetTuner_GoToCursor(I_GAIN_COL, OffsetTuner_Row);
			break;
	}
}

void OffsetTuner_MoveRightCursor() {
	switch (OffsetTuner_Col) {
		case GOAL_COL:
			OffsetTuner_GoToCursor(OFFSET_COL, OffsetTuner_Row);
			break;

		case OFFSET_COL:
			OffsetTuner_GoToCursor(MODVAL_COL, OffsetTuner_Row);
			break;

		case MODVAL_COL:
			OffsetTuner_GoToCursor(PRSVAL_COL, OffsetTuner_Row);
			break;

		case PRSVAL_COL:
			OffsetTuner_GoToCursor(ERRORS_COL, OffsetTuner_Row);
			break;

		case ERRORS_COL:
			OffsetTuner_GoToCursor(P_GAIN_COL, OffsetTuner_Row);
			break;

		case P_GAIN_COL:
			OffsetTuner_GoToCursor(I_GAIN_COL, OffsetTuner_Row);
			break;

		case I_GAIN_COL:
			OffsetTuner_GoToCursor(D_GAIN_COL, OffsetTuner_Row);
			break;
	}
}

void OffsetTuner_DrawPage() {
	int old_col = OffsetTuner_Col;
	int old_row = OffsetTuner_Row;

	OffsetTuner_ClearScreen();

	// 80              01234567890123456789012345678901234567890123456789012345678901234567890123456789     //24
	dbgu_print_ascii("ID: 1(R_SHO_PITCH)  [    ]        [    ]|                                      \r\n"); //0
	dbgu_print_ascii("ID: 2(L_SHO_PITCH)  [    ]        [    ]|                                      \r\n"); //1
	dbgu_print_ascii("ID: 3(R_SHO_ROLL)   [    ]        [    ]|                                      \r\n"); //2
	dbgu_print_ascii("ID: 4(L_SHO_ROLL)   [    ]        [    ]|                                      \r\n"); //3
	dbgu_print_ascii("ID: 5(R_ELBOW)      [    ]        [    ]|                                      \r\n"); //4
	dbgu_print_ascii("ID: 6(L_ELBOW)      [    ]        [    ]|                                      \r\n"); //5
	dbgu_print_ascii("ID: 7(R_HIP_YAW)    [    ]        [    ]|                                      \r\n"); //6
	dbgu_print_ascii("ID: 8(L_HIP_YAW)    [    ]        [    ]|                                      \r\n"); //7
	dbgu_print_ascii("ID: 9(R_HIP_ROLL)   [    ]        [    ]|                                      \r\n"); //8
	dbgu_print_ascii("ID:10(L_HIP_ROLL)   [    ]        [    ]|                                      \r\n"); //9
	dbgu_print_ascii("ID:11(R_HIP_PITCH)  [    ]        [    ]|                                      \r\n"); //0
	dbgu_print_ascii("ID:12(L_HIP_PITCH)  [    ]        [    ]|                                      \r\n"); //1
	dbgu_print_ascii("ID:13(R_KNEE)       [    ]        [    ]|                                      \r\n"); //2
	dbgu_print_ascii("ID:14(L_KNEE)       [    ]        [    ]|                                      \r\n"); //3
	dbgu_print_ascii("ID:15(R_ANK_PITCH)  [    ]        [    ]|                                      \r\n"); //4
	dbgu_print_ascii("ID:16(L_ANK_PITCH)  [    ]        [    ]|                                      \r\n"); //5
	dbgu_print_ascii("ID:17(R_ANK_ROLL)   [    ]        [    ]|                                      \r\n"); //6
	dbgu_print_ascii("ID:18(L_ANK_ROLL)   [    ]        [    ]|                                      \r\n"); //7
	dbgu_print_ascii("ID:19(HEAD_PAN)     [    ]        [    ]|                                      \r\n"); //8
	dbgu_print_ascii("ID:20(HEAD_TILT)    [    ]        [    ]|                                      \r\n"); //9
	dbgu_print_ascii("                     GOAL  OFFSET MODVAL PRSPOS ERRORS P_GAIN I_GAIN D_GAIN    \r\n"); //0
	dbgu_print_ascii("]                                                                              "); //1

	for (int i = 0; i <= 7; ++i)
		OffsetTuner_DrawStep(i);

	OffsetTuner_GoToCursor(old_col, old_row);
}

void OffsetTuner_DrawStep(int idx) {
	int old_col = OffsetTuner_Col;
	int old_row = OffsetTuner_Row;
	int col;

	switch (idx) {
		case 0:
			col = OFFSET_COL;
			for (int id = 1; id < SERVO_COUNT_MAX; id++) {
				OffsetTuner_GoToCursor(col, id - 1);
				dbgu_printf("%4d ", Servo_GetOffset(id));
			}
			break;

		case 1:
			col = MODVAL_COL;
			for (int id = 1; id < SERVO_COUNT_MAX; id++) {
				OffsetTuner_GoToCursor(col, id - 1);
				dbgu_printf("%4d", OffsetTuner_Step.position[id]);
			}
			break;

		case 2:
			col = PRSVAL_COL;
			for (int id = 1; id < SERVO_COUNT_MAX; id++) {
				OffsetTuner_GoToCursor(col, id - 1);
				if (OffsetTuner_PresentPos.position[id] & INVALID_BIT_MASK)
					dbgu_printf("----");
				else if (OffsetTuner_PresentPos.position[id] & TORQUE_OFF_BIT_MASK)
					dbgu_printf("????");
				else
					dbgu_printf("%.4d", OffsetTuner_PresentPos.position[id]);
			}
			break;

		case 3:
			col = ERRORS_COL;
			for (int id = 1; id < SERVO_COUNT_MAX; id++) {
				OffsetTuner_GoToCursor(col, id - 1);
				if (OffsetTuner_PresentPos.position[id] & INVALID_BIT_MASK)
					dbgu_printf("----");
				else if (OffsetTuner_PresentPos.position[id] & TORQUE_OFF_BIT_MASK)
					dbgu_printf("????");
				else
					dbgu_printf("%4d ", OffsetTuner_PresentPos.position[id] - OffsetTuner_Step.position[id]);
			}
			break;

		case 4:
			col = GOAL_COL;
			for (int id = 1; id < SERVO_COUNT_MAX; id++) {
				OffsetTuner_GoToCursor(col, id - 1);
				dbgu_printf("%4d", InitPose[id]);
			}
			break;

		case 5:
			col = P_GAIN_COL;
			for (int id = 1; id < SERVO_COUNT_MAX; id++) {
				OffsetTuner_GoToCursor(col, id - 1);
				dbgu_printf("%4d", OffsetTuner_P_Gain[id]);
			}
			break;

		case 6:
			col = I_GAIN_COL;
			for (int id = 1; id < SERVO_COUNT_MAX; id++) {
				OffsetTuner_GoToCursor(col, id - 1);
				dbgu_printf("%4d", OffsetTuner_I_Gain[id]);
			}
			break;

		case 7:
			col = D_GAIN_COL;
			for (int id = 1; id < SERVO_COUNT_MAX; id++) {
				OffsetTuner_GoToCursor(col, id - 1);
				dbgu_printf("%4d", OffsetTuner_D_Gain[id]);
			}
			break;

		default:
			return;
	}

	OffsetTuner_GoToCursor(old_col, old_row);
}

void OffsetTuner_UpDownValue(int offset) {
	if (OffsetTuner_Col == OFFSET_COL) {
/*		if ((InitPose[OffsetTuner_Row + 1] + OffsetTuner_GetValue() + offset) > SERVO_MAX_VALUE)
			OffsetTuner_SetValue(SERVO_MAX_VALUE - InitPose[OffsetTuner_Row + 1]);
		else if ((InitPose[OffsetTuner_Row + 1] + OffsetTuner_GetValue() + offset) < SERVO_MIN_VALUE)
			OffsetTuner_SetValue(SERVO_MIN_VALUE - InitPose[OffsetTuner_Row + 1]);
		else {*/
		OffsetTuner_SetValue(OffsetTuner_GetValue() + offset);
//		}
		OffsetTuner_bEdited = 1;
	} else
		OffsetTuner_SetValue(OffsetTuner_GetValue() + offset);
}

int OffsetTuner_GetValue() {
	int col;
	int row;
	if (OffsetTuner_bBeginCommandMode == 1) {
		col = OffsetTuner_Old_Col;
		row = OffsetTuner_Old_Row;
	} else {
		col = OffsetTuner_Col;
		row = OffsetTuner_Row;
	}

	if (col == GOAL_COL)
		return InitPose[row + 1];
	else if (col == OFFSET_COL)
		return Servo_GetOffset(row + 1);
	else if (col == MODVAL_COL)
		return OffsetTuner_Step.position[row + 1];
	else if (col == PRSVAL_COL)
		return OffsetTuner_PresentPos.position[row + 1];
	else if (col == ERRORS_COL)
		return OffsetTuner_PresentPos.position[row + 1] - OffsetTuner_Step.position[row + 1];
	else if (col == P_GAIN_COL)
		return OffsetTuner_P_Gain[row + 1];
	else if (col == I_GAIN_COL)
		return OffsetTuner_I_Gain[row + 1];
	else if (col == D_GAIN_COL)
		return OffsetTuner_D_Gain[row + 1];

	return -1;
}

void OffsetTuner_SetValue(int value) {
	int col;
	int row;
	if (OffsetTuner_bBeginCommandMode == 1) {
		col = OffsetTuner_Old_Col;
		row = OffsetTuner_Old_Row;
	} else {
		col = OffsetTuner_Col;
		row = OffsetTuner_Row;
	}

	OffsetTuner_GoToCursor(col, row);

	if (col == GOAL_COL) {
		//if (value + Servo_GetOffset(row + 1) >= 0 && value + Servo_GetOffset(row + 1) <= SERVO_MAX_VALUE) {
		if (!(OffsetTuner_Step.position[row + 1] & INVALID_BIT_MASK) && !(OffsetTuner_Step.position[row + 1] & TORQUE_OFF_BIT_MASK)) {
			Servo_SendPosition(row + 1, MOTION_VAL4096_TO_ANGLE(value));
			InitPose[row + 1] = value;
			dbgu_printf("%.4d", value);
			OffsetTuner_Step.position[row + 1] = value + Servo_GetOffset(row + 1);
			OffsetTuner_GoToCursor(MODVAL_COL, row);
			dbgu_printf("%.4d", OffsetTuner_Step.position[row + 1]);
		}
		//}
	} else if (col == OFFSET_COL) {
		Servo_SetOffset(row + 1, value);
		dbgu_printf("%4d ", OffsetTuner_GetValue());

		//if (InitPose[row + 1] + value >= 0 && InitPose[row + 1] + value <= SERVO_MAX_VALUE) {
		if (!(OffsetTuner_Step.position[row + 1] & INVALID_BIT_MASK) && !(OffsetTuner_Step.position[row + 1] & TORQUE_OFF_BIT_MASK)) {
			Servo_SendPosition(row + 1, MOTION_VAL4096_TO_ANGLE(InitPose[row + 1]));
			OffsetTuner_Step.position[row + 1] = InitPose[row + 1] + value;
			OffsetTuner_GoToCursor(MODVAL_COL, row);
			dbgu_printf("%.4d", OffsetTuner_Step.position[row + 1]);
		}
		//}
		OffsetTuner_bEdited = 1;
	} else if (col == MODVAL_COL) {
		//if (value >= SERVO_MIN_VALUE && value <= SERVO_MAX_VALUE) {
		if (!(OffsetTuner_Step.position[row + 1] & INVALID_BIT_MASK) && !(OffsetTuner_Step.position[row + 1] & TORQUE_OFF_BIT_MASK)) {

			Servo_SetOffsetEnable(0);
			Servo_SendPosition(row + 1, MOTION_VAL4096_TO_ANGLE(value));
			Servo_SetOffsetEnable(1);

			OffsetTuner_Step.position[row + 1] = value;
			dbgu_printf("%.4d", OffsetTuner_Step.position[row + 1]);
			Servo_SetOffset(row + 1, value - InitPose[row + 1]);
			OffsetTuner_GoToCursor(OFFSET_COL, row);
			dbgu_printf("%4d ", Servo_GetOffset(row + 1));
		}
		//}
	} else if (col == PRSVAL_COL) {
		dbgu_printf("%.4d", OffsetTuner_GetValue());
	} else if (col == ERRORS_COL) {
		dbgu_printf("%.4d ", OffsetTuner_GetValue());
	} else if (col == P_GAIN_COL) {
		if (value >= 0 && value <= 254) {
			if (!(OffsetTuner_Step.position[row + 1] & INVALID_BIT_MASK) && !(OffsetTuner_Step.position[row + 1] & TORQUE_OFF_BIT_MASK)) {
				//if(cm730->WriteByte(row + 1, MX28::P_P_GAIN, value, &error) == CM730::SUCCESS)
				//{
				OffsetTuner_P_Gain[row + 1] = value;
				dbgu_printf("%4d", OffsetTuner_P_Gain[row + 1]);
				//}
			}
		}
	} else if (col == I_GAIN_COL) {
		if (value >= 0 && value <= 254) {
			if (!(OffsetTuner_Step.position[row + 1] & INVALID_BIT_MASK) && !(OffsetTuner_Step.position[row + 1] & TORQUE_OFF_BIT_MASK)) {
				//if(cm730->WriteByte(row + 1, MX28::P_I_GAIN, value, &error) == CM730::SUCCESS)
				//{
				OffsetTuner_I_Gain[row + 1] = value;
				dbgu_printf("%4d", OffsetTuner_I_Gain[row + 1]);
				//}
			}
		}
	} else if (col == D_GAIN_COL) {
		if (value >= 0 && value <= 254) {
			if (!(OffsetTuner_Step.position[row + 1] & INVALID_BIT_MASK) && !(OffsetTuner_Step.position[row + 1] & TORQUE_OFF_BIT_MASK)) {
				//if(cm730->WriteByte(row + 1, MX28::P_D_GAIN, value, &error) == CM730::SUCCESS)
				//{
				OffsetTuner_D_Gain[row + 1] = value;
				dbgu_printf("%4d", OffsetTuner_D_Gain[row + 1]);
				//}
			}
		}
	}

	OffsetTuner_GoToCursor(col, row);
}

void OffsetTuner_ToggleTorque() {
	if ((OffsetTuner_Col != GOAL_COL && OffsetTuner_Col != MODVAL_COL) || OffsetTuner_Row > ID_20_ROW)
		return;

	int id = OffsetTuner_Row + 1;

	if (OffsetTuner_Step.position[id] & TORQUE_OFF_BIT_MASK) {
		if (Servo_SetPassive(id, 0) != 1)
			return;

		int value = 0;

		if (OffsetTuner_Col == MODVAL_COL) {
			value = MOTION_ANGLE_TO_VAL4096(Servo_GetPosition(id)) + Servo_GetOffset(id);
			OffsetTuner_Step.position[id] = value;
		} else if (OffsetTuner_Col == PRSVAL_COL) {
			value = MOTION_ANGLE_TO_VAL4096(Servo_GetPosition(id)) + Servo_GetOffset(id);
			OffsetTuner_Step.position[id] = value;

			value = Servo_ReadPos(id);
			if (value != -1000) {
				OffsetTuner_PresentPos.position[id] = MOTION_ANGLE_TO_VAL4096(value);
			}
		} else if (OffsetTuner_Col == GOAL_COL) {
			value = MOTION_ANGLE_TO_VAL4096(Servo_GetPosition(id)) + Servo_GetOffset(id);
			OffsetTuner_Step.position[id] = value;
			InitPose[id] = value = OffsetTuner_Step.position[id] - Servo_GetOffset(id);
		}

		//dbgu_printf("%.4d", value);
		OffsetTuner_SetValue(value);
	} else {
		if (Servo_SetPassive(id, 1) != 1)
			return;

		OffsetTuner_Step.position[id] = TORQUE_OFF_BIT_MASK;
		dbgu_printf("????");
	}

	OffsetTuner_GoToCursor(OffsetTuner_Col, OffsetTuner_Row);
}

void OffsetTuner_BeginCommandMode() {
	OffsetTuner_Old_Col = OffsetTuner_Col;
	OffsetTuner_Old_Row = OffsetTuner_Row;
	OffsetTuner_ClearCmd();
	OffsetTuner_GoToCursor(CMD_COL, CMD_ROW);
	OffsetTuner_bBeginCommandMode = 1;
}

void OffsetTuner_EndCommandMode() {
	OffsetTuner_GoToCursor(OffsetTuner_Old_Col, OffsetTuner_Old_Row);
	OffsetTuner_bBeginCommandMode = 0;
}

void OffsetTuner_HelpCmd() {
	OffsetTuner_ClearScreen();
	dbgu_printf(" exit               Exits the program.\r\n");
	dbgu_printf(" re                 Refreshes the screen.\r\n");
	dbgu_printf(" set [value]        Sets value on cursor [value].\r\n");
	dbgu_printf(" pgain [value]      Sets ALL actuators' P gain to [value].\r\n");
	dbgu_printf(" igain [value]      Sets ALL actuators' I gain to [value].\r\n");
	dbgu_printf(" dgain [value]      Sets ALL actuators' D gain to [value].\r\n");
	dbgu_printf(" save               Saves offset changes.\r\n");
	dbgu_printf(" on/off             Turn On/Off torque from ALL actuators.\r\n");
	dbgu_printf(" on/off [index1] [index2] ...  \r\n"
			"                    turns On/Off torque from ID[index1] ID[index2]...\r\n");
	dbgu_printf("\r\n");
	dbgu_printf(" Press any key to continue...");
	dbgu_get_char();

	OffsetTuner_DrawPage();
}

void OffsetTuner_ReadStep() {
	for (int id = 0; id < 31; id++) {
		if (id >= MIN_SERVO_ID && id < SERVO_COUNT_MAX) {
			if (Servo_GetPassive(id) == 0) {
				Servo_SetOffsetEnable(0);
				float value = Servo_ReadPos(id);
				Servo_SetOffsetEnable(1);
				if (value != -1000) {
					OffsetTuner_Step.position[id] = MOTION_ANGLE_TO_VAL4096(Servo_GetPosition(id)) + Servo_GetOffset(id);
					OffsetTuner_PresentPos.position[id] = MOTION_ANGLE_TO_VAL4096(value);
					//OffsetTuner_P_Gain[id] = table[MX28::P_P_GAIN];
					//OffsetTuner_I_Gain[id] = table[MX28::P_I_GAIN];
					//OffsetTuner_D_Gain[id] = table[MX28::P_D_GAIN];
				} else
					OffsetTuner_Step.position[id] = INVALID_BIT_MASK;
			} else
				OffsetTuner_Step.position[id] = TORQUE_OFF_BIT_MASK;
		} else
			OffsetTuner_Step.position[id] = INVALID_BIT_MASK;
	}
}

unsigned char OffsetTuner_AskSave() {
	if (OffsetTuner_bEdited == 1) {
		OffsetTuner_PrintCmd("Are you sure? (y/n)");
		if (dbgu_get_char() != 'y') {
			OffsetTuner_ClearCmd();
			return 1;
		}
	}

	return 0;
}

void OnOffCmd(unsigned char on, int num_param, int *list) {
	if (num_param == 0) {
		for (int id = 1; id < SERVO_COUNT_MAX; id++)
			Servo_SetPassive(id, 1 - on);
	} else {
		for (int i = 0; i < num_param; i++) {
			if (list[i] >= 0 && list[i] < SERVO_COUNT_MAX)
				Servo_SetPassive(list[i], 1 - on);
		}
	}

	OffsetTuner_ReadStep();
	//OffsetTuner_DrawStep(7);
	OffsetTuner_DrawPage();
}

void GainCmd(int value, int pid_col) {
	if (value < 0 || value > 254) {
		OffsetTuner_PrintCmd("Invalid gain value");
		return;
	}

	if (pid_col == P_GAIN_COL) {
		//cm730->SyncWrite(MX28::P_P_GAIN, 2, JointData::NUMBER_OF_JOINTS - 1, param);
		OffsetTuner_DrawStep(5);
	} else if (pid_col == I_GAIN_COL) {
		//cm730->SyncWrite(MX28::P_I_GAIN, 2, JointData::NUMBER_OF_JOINTS - 1, param);
		OffsetTuner_DrawStep(6);
	} else if (pid_col == D_GAIN_COL) {
		//cm730->SyncWrite(MX28::P_D_GAIN, 2, JointData::NUMBER_OF_JOINTS - 1, param);
		OffsetTuner_DrawStep(7);
	}
}

void OffsetTuner_SaveCmd() {
	Config_Save();
	OffsetTuner_bEdited = 0;
}

void OffsetTuner_Main(void) {
	OffsetTuner_Col = OFFSET_COL;
	OffsetTuner_Row = ID_1_ROW;
	OffsetTuner_Old_Col = OffsetTuner_Col;
	OffsetTuner_Old_Row = OffsetTuner_Row;

	for (int id = 1; id < SERVO_COUNT_MAX; id++) {
		Servo_SetPosition(id, MOTION_VAL4096_TO_ANGLE(InitPose[id]));
	}
	Servo_SendPositions();

	Action_LoadPage(OffsetTuner_indexPage, &OffsetTuner_Page);

	OffsetTuner_ReadStep();
	OffsetTuner_Step.pause = 0;
	OffsetTuner_Step.time = 0;

	OffsetTuner_DrawPage();

	while (1) {
		unsigned long loopStartTime = TC_GetSystemTicks();

		UI_ToggleLED(0);

		if (dbgu_char_available() == 1) {
			int ch = dbgu_get_char();
			if (ch == 0x48 || ch == 0x41) // Up arrow key
				OffsetTuner_MoveUpCursor();
			else if (ch == 0x50 || ch == 0x42) // Down arrow key
				OffsetTuner_MoveDownCursor();
			else if (ch == 0x4b || ch == 0x44) // Left arrow keyw
				OffsetTuner_MoveLeftCursor();
			else if (ch == 0x4d || ch == 0x43)
				OffsetTuner_MoveRightCursor();
			else if (ch == ',')
				OffsetTuner_MoveUpCursor();
			else if (ch == '.')
				OffsetTuner_MoveDownCursor();
			else if (ch == '-')
				OffsetTuner_UpDownValue(-1);
			else if (ch == '+')
				OffsetTuner_UpDownValue(1);
			else if (ch == '_')
				OffsetTuner_UpDownValue(-10);
			else if (ch == '*')
				OffsetTuner_UpDownValue(10);
			else if (ch == ' ')
				OffsetTuner_ToggleTorque();
			else if (ch >= 'A' && ch <= 'z') {
				char input[128] = { 0, };
				char *token;
				int input_len;
				char cmd[80];
				int iparam[30];

				int idx = 0;

				OffsetTuner_BeginCommandMode();

				dbgu_printf("%c", ch);
				input[idx++] = (char) ch;

				while (1) {
					ch = dbgu_get_char();
					if (ch == 0x0D || ch == 0x0A)
						break;
					else if (ch == 0x08 || ch == 0x7F) {
						if (idx > 0) {
							ch = 0x08;
							dbgu_printf("%c", ch);
							ch = ' ';
							dbgu_printf("%c", ch);
							ch = 0x08;
							dbgu_printf("%c", ch);
							input[--idx] = 0;
						}
					} else if (ch >= 'A' && ch <= 'z') {
						if (idx < 127) {
							dbgu_printf("%c", ch);
							input[idx++] = (char) ch;
						}
					}
				}

				//fflush(stdin);
				input_len = strlen(input);
				if (input_len > 0) {
					token = strtok(input, " ");
					if (token != 0) {
						strcpy(cmd, token);
						token = strtok(0, " ");
						int num_param = 0;
						while (token != 0) {
							iparam[num_param++] = atoi(token);
							token = strtok(0, " ");
						}

						if (strcmp(cmd, "exit") == 0) {
							if (OffsetTuner_AskSave() == 0)
								break;
						} else if (strcmp(cmd, "re") == 0) {
							OffsetTuner_ReadStep();
							OffsetTuner_DrawPage();
						} else if (strcmp(cmd, "help") == 0)
							OffsetTuner_HelpCmd();
						else if (strcmp(cmd, "set") == 0) {
							if (num_param > 0)
								OffsetTuner_SetValue(iparam[0]);
							else
								OffsetTuner_PrintCmd("Need parameter");
						} else if (strcmp(cmd, "on") == 0)
							OnOffCmd(1, num_param, iparam);
						else if (strcmp(cmd, "off") == 0)
							OnOffCmd(0, num_param, iparam);
						else if (strcmp(cmd, "save") == 0)
							OffsetTuner_SaveCmd();
						else if (strcmp(cmd, "pgain") == 0) {
							if (num_param > 0)
								GainCmd(iparam[0], P_GAIN_COL);
							else
								OffsetTuner_PrintCmd("Need parameter");
						} else if (strcmp(cmd, "igain") == 0) {
							if (num_param > 0)
								GainCmd(iparam[0], I_GAIN_COL);
							else
								OffsetTuner_PrintCmd("Need parameter");
						} else if (strcmp(cmd, "dgain") == 0) {
							if (num_param > 0)
								GainCmd(iparam[0], D_GAIN_COL);
							else
								OffsetTuner_PrintCmd("Need parameter");
						} else
							OffsetTuner_PrintCmd("Bad command! please input 'help'");
					}
				}

				OffsetTuner_EndCommandMode();
			}
		}

		// Waste remaining loop time
		//if (TC_GetMsSinceTick(loopStartTime) > MAIN_LOOP_DURATION) {
		//	dbgu_printf("[Main] WARNING: Main loop has taken too long: %d ms instead of %d ms!\r\n", TC_GetMsSinceTick(loopStartTime), MAIN_LOOP_DURATION);
		//}
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION) {
		}
	}
}
