/*
 * imu.c
 *
 *  Created on: 23.03.2011
 *      Author: Stefan
 */

#include "imu.h"
#include "imu/imu.h"
#include "include/Board.h"
#include "ui/ui.h"
#include "tc/tc.h"
#include "dbgu/dbgu.h"
#include <string.h>

#define MAIN_LOOP_DURATION		10 // ms

void IMUTest_Main(void) {
	while (1) {
		unsigned long loopStartTime = TC_GetSystemTicks();

		UI_ToggleLED(1);
		IMU_Update();

		// Send IMU data via DBGU
		float accel_x = IMU_GetFBAccel();
		float accel_y = IMU_GetRLAccel();
		float accel_z = 0; //IMU_GetAccelZ();
		//float accel_x_av = IMU_GetFBAccelAverage();
		//float accel_y_av = IMU_GetRLAccelAverage();
		float roll = IMU_GetRollRaw();
		float pitch = IMU_GetPitchRaw();
		float gyro_x = IMU_GetFBGyro();
		float gyro_y = IMU_GetRLGyro();
		float roll_filt = IMU_GetRoll();
		float pitch_filt = IMU_GetPitch();
		dbgu_printf("%3.5f;%3.5f;%3.5f;%3.5f;%3.5f;%3.5f;%3.5f;%3.5f;%3.5f\r\n", accel_x, accel_y, accel_z, roll, pitch, gyro_x, gyro_y, roll_filt, pitch_filt);
		//dbgu_printf("%3.5f;%3.5f; %3.5f;%3.5f; %3.5f;%3.5f\r\n", accel_x, accel_y, accel_x_av, accel_y_av, gyro_x, gyro_y);

		if (dbgu_char_available()) {
			int ch = dbgu_get_char();
			if( ch == 'c') {
				IMU_Calibrate_Accel();
			} else if( ch == 3) { // Ctrl + C
				return;
			}
		}

		// Waste remaining loop time
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION);
	}
}
