#include "controller/controller.h"
#include "tc/tc.h"
#include "dbgu/dbgu.h"

struct PID_data {
	float 			KP;
	float 			KI;
	float 			KD;
	float			SetPoint;

	float			ErrorSum;
	float			LastError;
	unsigned long	LastUpdate;

	float			Adjust;
};

struct PID_data pid_X;					// Controller for hip angle (controls angle of upper body)

void PID_Init(void) {
	pid_X.KP = 0.0001; //0.24;
	pid_X.KI = 0.0005; //0.9;
	pid_X.KD = 0.0001; //0.04;
	pid_X.SetPoint = LEAN_FORWARD_ANGLE;// Angle of upper body
	pid_X.LastUpdate = 0;

	PID_Reset();
}

void PID_Reset(void){
	pid_X.ErrorSum = 0.0;
	pid_X.LastError = 0.0;
	pid_X.Adjust = 0.0;
	pid_X.LastUpdate = TC_GetSystemTicks();
}

float PID_Update(struct PID_data* data, float curValue) {
	float deltaTime = (float)TC_GetMsSinceTick(data->LastUpdate) / 1000.0;
	data->LastUpdate = TC_GetSystemTicks();
	if (deltaTime == 0) deltaTime = 1;

	float error = data->SetPoint - curValue;
	data->ErrorSum += error;

	// Anti wind-up
	if (data->ErrorSum < -800) {
		data->ErrorSum = -800;
	} else if (data->ErrorSum > 800) {
		data->ErrorSum = 800;
	}

	float deltaErrorD = error - data->LastError;
	float adjust = (data->KP * error) + ((data->KI * deltaTime) * data->ErrorSum) + (data->KD * (deltaErrorD)/(deltaTime));
	//Limit adjustment
	if (adjust < -20.0) {
		adjust = -20.0;
	} else if (adjust > 20.0) {
		adjust = 20.0;
	}
	//dbgu_printf("error: %d errorsum: %d adjust: %5.3f lage: %d ki %5.3f kd %5.3f\n\r", error, data->ErrorSum, adjust, actValue, data->KI, data->KD);

	data->LastError = error;
	data->Adjust = adjust;

	return adjust;
}

float PID_UpdateX(float actual) {
	return PID_Update(&pid_X, actual);
}

float PID_GetAdjustX(void) {
	return pid_X.Adjust;
}

void PID_SetParametersX(float P, float I, float D, int target) {
	pid_X.KP = P;
	pid_X.KI = I;
	pid_X.KD = D;
	pid_X.SetPoint = target;
//	dbgu_printf("Setting X parameters: P: %5.3f I: %5.3f D: %5.3f SetPoint: %u\r\n", P, I, D, soll);
}
