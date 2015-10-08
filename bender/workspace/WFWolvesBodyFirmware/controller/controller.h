
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define LEAN_FORWARD_ANGLE		7 // Degrees

void PID_Init(void);
void PID_Reset(void);

float PID_UpdateX(float actual);
float PID_GetAdjustX(void);

void PID_SetParametersX(float P, float I, float D, int target);

#endif /*CONTROLLER_H_*/
