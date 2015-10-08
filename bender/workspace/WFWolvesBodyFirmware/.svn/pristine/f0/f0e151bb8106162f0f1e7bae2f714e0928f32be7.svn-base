#ifndef IMU_H_
#define IMU_H_

struct imuData_t {
	int fb_accel;
	int rl_accel;
	int fb_gyro;
	int rl_gyro;
} __attribute__((packed));

void IMU_Init(void);
unsigned char IMU_Calibrate(void);
void IMU_Calibrate_Accel();
void IMU_Update(void);

int IMU_GetFBGyro(void);
int IMU_GetRLGyro(void);
int IMU_GetFBAccel(void);
int IMU_GetFBAccelAverage(void);
int IMU_GetFBAccelAverageC(void);
int IMU_GetRLAccel(void);
int IMU_GetRLAccelAverage(void);
int IMU_GetRLAccelAverageC(void);
int IMU_GetFBMagnet(void);
int IMU_GetRLMagnet(void);
int IMU_GetUDMagnet(void);

float IMU_GetPitch(void);
float IMU_GetRoll(void);
float IMU_GetPitchRaw(void);
float IMU_GetRollRaw(void);

double IMU_get_temp(void);
double IMU_CalculateHeadingTiltCompensated(void);

void IMU_UpdateStatus(struct imuData_t* data);

#endif /*IMU_H_*/
