#include "imu.h"
#include "adc/adc.h"
#include "tc/tc.h"
#include "kalman/kalman.h"
#include "dbgu/dbgu.h"
#include "i2c/i2c.h"
#include <math.h>

#define ADXL_ADDRESS					0x53
#define HMC_ADDRESS						0x1E
#define ITG_ADDRESS						0x68

#define GYRO_WINDOW_SIZE    			100
#define ACCEL_WINDOW_SIZE   			30 //10
#define MARGIN_OF_SD        			2.0 //2.0
#define CALIBRATE_UPDATE_INTERVAL		8 // ms

//#define USE_KALMAN_FILTER

#define R_ACCEL		0.2 //0.7		//0.2			// Noise range of accelerometer
#define Q_ACCEL		0.5 // 0.0001 	//0.5
#define Q_GYRO		0.3// 0.0003		//0.3

#define RAD_TO_DEG_FACTOR				57.2957795


#define ADXL_BW_RATE		0x2c
#define ADXL_POWER_CTL		0x2d
#define ADXL_DATA_FORMAT	0x31
#define ITG_DLPF_FS			0x16
#define ITG_PWR_MGM			0x3e
#define HMC_CONFIG_A		0x00
#define HMC_CONFIG_B		0x01
#define HMC_MODE			0x02

unsigned char imu_digital;

int imu_FBAccelCenter = 512;
int imu_RLAccelCenter = 512;
int imu_FBGyroCenter = 512;
int imu_RLGyroCenter = 512;

int imu_FBGyro = 0;
int imu_RLGyro = 0;
int imu_FBAccel = 0;
int imu_RLAccel = 0;
int imu_FBAccelAverage = 0;
int imu_RLAccelAverage = 0;
int imu_FBMagnet = 0;
int imu_RLMagnet = 0;
int imu_UDMagnet = 0;

int imu_AccelX_raw, imu_AccelY_raw, imu_AccelZ_raw;
int imu_GyroX_raw, imu_GyroY_raw, imu_GyroZ_raw;
int imu_MagnetX_raw, imu_MagnetY_raw, imu_MagnetZ_raw;
int imu_MagnetX_cal, imu_MagnetY_cal, imu_MagnetZ_cal;

#ifdef USE_KALMAN_FILTER
struct Gyro1DKalman filter_roll;
struct Gyro1DKalman filter_pitch;
unsigned long imu_lastUpdate;
#endif

float roll = 0;
float pitch = 0;
float roll_output = 0;
float pitch_output = 0;

void IMU_accel_init(void);
void IMU_accel_update(void);
void IMU_gyro_init(void);
void IMU_gyro_update(void);
void IMU_magnet_init(void);
void IMU_magnet_update(void);

void IMU_Init(void) {
	// Check which IMU is installed by trying to read ID registers
	unsigned char id_adxl = I2C_ReadRegister(ADXL_ADDRESS, 0x00);
	unsigned char id_itg = I2C_ReadRegister(ITG_ADDRESS, 0x00);
	unsigned char id_hmc[3];
	I2C_ReadMultipleRegisters(HMC_ADDRESS, 0x0A, &id_hmc[0], 3);
	if ((id_adxl == 0xe5) &&
			((id_itg & 0x7e) == ITG_ADDRESS) &&
			(id_hmc[0] == 'H' && id_hmc[1] == '4' && id_hmc[2] == '3')) {
		dbgu_printf("Found digital IMU!\r\n");

		IMU_accel_init();
		IMU_gyro_init();
		IMU_magnet_init();

		TC_DelayMs(100);
		double temp = IMU_get_temp();
		dbgu_printf("Current temperature: %f °C\r\n", temp);

		imu_digital = 1;
	} else {
		imu_digital = 0;
	}

	// Todo: load values from eeprom
	IMU_Update();
	IMU_Calibrate_Accel();

#ifdef USE_KALMAN_FILTER
	init_Gyro1DKalman(&filter_roll, Q_ACCEL, Q_GYRO, R_ACCEL);
   	init_Gyro1DKalman(&filter_pitch, Q_ACCEL, Q_GYRO, R_ACCEL);

	float accX = ((float)(imu_AccelZ_raw - 512)) / 128.0f;
	float accY = ((float)(imu_AccelY_raw - 512)) / 128.0f;
	float accZ = ((float)(imu_AccelX_raw - 512)) / 128.0f;
	pitch = atan2(accX, sqrt(accY * accY + accZ * accZ));
	roll = atan2(accY, -accZ);
	filter_roll.x_angle = roll;
	filter_pitch.x_angle = pitch;
#endif
}

void IMU_Calibrate_Accel()
{
	imu_RLAccelCenter = imu_RLAccelAverage;
	imu_FBAccelCenter = imu_FBAccelAverage;
}

unsigned char IMU_Calibrate(void){
    int fb_gyro_array[GYRO_WINDOW_SIZE] = {512,};
    int rl_gyro_array[GYRO_WINDOW_SIZE] = {512,};

    // Measure data
    for (int i = 0; i < GYRO_WINDOW_SIZE; i++) {
		unsigned long loopStartTime = TC_GetSystemTicks();
		if (imu_digital == 1) {
			IMU_gyro_update();
			fb_gyro_array[i] = imu_GyroY_raw;
			rl_gyro_array[i] = imu_GyroZ_raw;
		} else {
			fb_gyro_array[i] = ADC_GetChannel(1);
			rl_gyro_array[i] = ADC_GetChannel(2);
		}
		while (TC_GetMsSinceTick(loopStartTime) < CALIBRATE_UPDATE_INTERVAL);
	}

    double fb_sum = 0.0, rl_sum = 0.0;
    double fb_sd = 0.0, rl_sd = 0.0;
    double fb_diff, rl_diff;
    double fb_mean = 0.0, rl_mean = 0.0;

	for (int i = 0; i < GYRO_WINDOW_SIZE; i++) {
		fb_sum += fb_gyro_array[i];
		rl_sum += rl_gyro_array[i];
	}
	fb_mean = fb_sum / GYRO_WINDOW_SIZE;
	rl_mean = rl_sum / GYRO_WINDOW_SIZE;

    fb_sum = 0.0;
	rl_sum = 0.0;
	for (int i = 0; i < GYRO_WINDOW_SIZE; i++) {
		fb_diff = fb_gyro_array[i] - fb_mean;
		rl_diff = rl_gyro_array[i] - rl_mean;
		fb_sum += fb_diff * fb_diff;
		rl_sum += rl_diff * rl_diff;
	}
	fb_sd = sqrt(fb_sum / GYRO_WINDOW_SIZE);
	rl_sd = sqrt(rl_sum / GYRO_WINDOW_SIZE);

    if (fb_sd < MARGIN_OF_SD && rl_sd < MARGIN_OF_SD) {
		imu_FBGyroCenter = (int)fb_mean;
		imu_RLGyroCenter = (int)rl_mean;
		//dbgu_printf("[IMU] FBGyroCenter: %d, RLGyroCenter: %d\r\n", imu_FBGyroCenter, imu_RLGyroCenter);
		return 1;
	} else {
		dbgu_printf("[IMU] Calibration failed, error margin too high!\r\n");
		imu_FBGyroCenter = 512;
		imu_RLGyroCenter = 512;
		return 0;
	}
}

void IMU_Update(void) {
    static int fb_array[ACCEL_WINDOW_SIZE] = {512,};
    static int rl_array[ACCEL_WINDOW_SIZE] = {512,};
    static int buf_idx = 0;

    int sum, idx;

    if (imu_digital == 1) {
    	IMU_accel_update();
    	IMU_gyro_update();
    	IMU_magnet_update();
		imu_FBGyro = imu_GyroY_raw - imu_FBGyroCenter;
		imu_RLGyro = imu_GyroZ_raw - imu_RLGyroCenter;
		imu_FBAccel = imu_AccelZ_raw;// - imu_FBAccelCenter;
		imu_RLAccel = imu_AccelY_raw;// - imu_RLAccelCenter;
		imu_FBMagnet = imu_MagnetX_raw;
		imu_RLMagnet = imu_MagnetY_raw;
		imu_UDMagnet = imu_MagnetZ_raw;

    } else {
		// Our gyro: +-500°/sec, DARwIn gyro: +-2000°/sec
		imu_FBGyro = (int)ADC_GetChannel(1) - imu_FBGyroCenter;
		imu_RLGyro = (int)ADC_GetChannel(2) - imu_RLGyroCenter;
		imu_FBAccel = (int)ADC_GetChannel(3);// - imu_FBAccelCenter;
		imu_RLAccel = (int)ADC_GetChannel(4);// - imu_RLAccelCenter;
    }

	fb_array[buf_idx] = imu_FBAccel;

    sum = 0;
    for(idx = 0; idx < ACCEL_WINDOW_SIZE; idx++) {
        sum += fb_array[idx];
    }
    imu_FBAccelAverage = sum / ACCEL_WINDOW_SIZE;

	rl_array[buf_idx] = imu_RLAccel;

    sum = 0;
    for(idx = 0; idx < ACCEL_WINDOW_SIZE; idx++) {
        sum += rl_array[idx];
    }
    imu_RLAccelAverage = sum / ACCEL_WINDOW_SIZE;

    if( ++buf_idx >= ACCEL_WINDOW_SIZE) {
    	buf_idx = 0;
    }

    //dbgu_printf("[IMU] FBGyro: %d RLGyro: %d RLAccel: %d FBAccelAvg: %d\r\n", imu_FBGyro, imu_RLGyro, imu_RLAccel, imu_FBAccelAverage);

#ifdef USE_KALMAN_FILTER
	float dt = TC_GetMsSinceTick(imu_lastUpdate) / 1000.0;
	imu_lastUpdate = TC_GetSystemTicks();

	//dbgu_printf("IMU_Update: dt=%5.3f\r\n", dt);

	// Convert acceleration to G
	float accX = ((float)(imu_AccelZ_raw - 512)) / 128.0f;
	float accY = ((float)(imu_AccelY_raw - 512)) / 128.0f;
	float accZ = ((float)(imu_AccelX_raw - 512)) / 128.0f;

//	dbgu_printf("IMU_Update: X=%11.8f, Y=%11.8f, Z=%11.8f\r\n", accX, accY, accZ);

	pitch = atan2(accX, sqrt(accY * accY + accZ * accZ));
	roll = atan2(accY, -accZ);

	float roll_rate = imu_RLGyro;
	float pitch_rate = imu_FBGyro;

	ars_predict(&filter_roll, roll_rate, dt);    // Kalman predict
	float roll_est = ars_update(&filter_roll, roll);

	ars_predict(&filter_pitch, pitch_rate, dt);    // Kalman predict
	float pitch_est = ars_update(&filter_pitch, pitch);

	// Report the estimate pitch and roll values in degrees.
	roll_output  = (roll_est * RAD_TO_DEG_FACTOR);
	pitch_output = (pitch_est * RAD_TO_DEG_FACTOR);

	//dbgu_printf("IMU_Update: roll=%d°, pitch=%d°\r\n", roll_output, pitch_output);
#endif
}

int IMU_GetFBAccelAverage(void) {
	return imu_FBAccelAverage;
}

int IMU_GetRLAccelAverage(void) {
	return imu_RLAccelAverage;
}

int IMU_GetFBAccelAverageC(void) {
	return imu_FBAccelAverage - imu_FBAccelCenter;
}

int IMU_GetRLAccelAverageC(void) {
	return imu_RLAccelAverage - imu_RLAccelCenter;
}

int IMU_GetFBGyro(void) {
	return imu_FBGyro;
}

int IMU_GetRLGyro(void) {
	return imu_RLGyro;
}

int IMU_GetFBAccel(void) {
	return imu_FBAccel/* - imu_FBAccelCenter*/;
}

int IMU_GetRLAccel(void) {
	return imu_RLAccel/* - imu_RLAccelCenter*/;
}

int IMU_GetFBMagnet(void)
{
	return imu_FBMagnet;
}

int IMU_GetRLMagnet(void)
{
	return imu_RLMagnet;
}

int IMU_GetUDMagnet(void)
{
	return imu_UDMagnet;
}

float IMU_GetPitch(void){
	return pitch_output;
}

float IMU_GetRoll(void){
	return roll_output;
}

float IMU_GetPitchRaw(void){
	return (pitch * RAD_TO_DEG_FACTOR);
}

float IMU_GetRollRaw(void){
	return (roll * RAD_TO_DEG_FACTOR);
}

void IMU_accel_init(void) {
	I2C_WriteRegister(ADXL_ADDRESS, ADXL_BW_RATE, 0x0e); // 1600 Hz ODR, 800 Hz Bandwidth
	I2C_WriteRegister(ADXL_ADDRESS, ADXL_POWER_CTL, 0x08);
	I2C_WriteRegister(ADXL_ADDRESS, ADXL_DATA_FORMAT, 0x09); // Full res, +-4g
}

void IMU_accel_update(void) {
	signed int temp;
	unsigned char data[6];
	I2C_ReadMultipleRegisters(ADXL_ADDRESS, 0x32, &data[0], 6);

#ifdef DYNAMIXEL
	imu_AccelZ_raw = (signed short)(data[0] | (data[1] << 8));
	imu_AccelY_raw = (signed short)(data[2] | (data[3] << 8)); // Platine gedreht, daher Achsen vertauscht
	imu_AccelX_raw = (signed short)(data[4] | (data[5] << 8)); //
#else
	imu_AccelX_raw = (signed short)(data[0] | (data[1] << 8));
	imu_AccelY_raw = (signed short)(data[2] | (data[3] << 8));
	imu_AccelZ_raw = (signed short)(data[4] | (data[5] << 8));
#endif
	// convert data s16 -> u10
	temp = (-1)*(imu_AccelX_raw / 2);
	temp = 512 + temp;
	if (temp > 1023 ) temp = 1023;
	if (temp < 0 ) temp = 0;
	imu_AccelX_raw = temp;

	// convert data s16 -> u10
	temp = (-1)*(imu_AccelY_raw / 2);
	temp = 512 + temp;
	if (temp > 1023 ) temp = 1023;
	if (temp < 0 ) temp = 0;
	imu_AccelY_raw = temp;

	// convert data s16 -> u10
	temp = (-1)*(imu_AccelZ_raw / 2);
	temp = 512 + temp;
	if (temp > 1023 ) temp = 1023;
	if (temp < 0 ) temp = 0;
	imu_AccelZ_raw = temp;

	//dbgu_printf("ACCEL: %d, %d, %d\r\n", imu_AccelX_raw, imu_AccelY_raw, imu_AccelZ_raw);
}

void IMU_gyro_init(void) {
	I2C_WriteRegister(ITG_ADDRESS, ITG_PWR_MGM, 0x80); // H_RESET
	I2C_WriteRegister(ITG_ADDRESS, ITG_DLPF_FS, 0x18); // DLPF_CFG = 0, FS_SEL = 3
	I2C_WriteRegister(ITG_ADDRESS, ITG_PWR_MGM, 0x00); // RESET off
}

void IMU_gyro_update(void) {
	signed int temp;
	unsigned char data[6];
	I2C_ReadMultipleRegisters(ITG_ADDRESS, 0x1d, &data[0], 6);

#ifdef DYNAMIXEL
	imu_GyroZ_raw = (signed short)(data[1] | (data[0] << 8));
	imu_GyroY_raw = (signed short)(data[3] | (data[2] << 8)); // Lage der Platine verändert,
	imu_GyroX_raw = (signed short)(data[5] | (data[4] << 8)); // daher Werte vertauscht
#else
	imu_GyroX_raw = (signed short)(data[1] | (data[0] << 8));
	imu_GyroY_raw = (signed short)(data[3] | (data[2] << 8));
	imu_GyroZ_raw = (signed short)(data[5] | (data[4] << 8));
#endif
	// convert data s16 -> u10
	temp = (imu_GyroX_raw / 64);
	temp = temp * 5 / 4;
	temp = 512 + temp;
	if (temp > 1023 ) temp = 1023;
	if (temp < 0 ) temp = 0;
	imu_GyroX_raw = temp;

	// convert data s16 -> u10
	temp = (imu_GyroY_raw / 64);
	temp = temp * 5 / 4;
	temp = 512 + temp;
	if (temp > 1023 ) temp = 1023;
	if (temp < 0 ) temp = 0;
	imu_GyroY_raw = temp;

	// convert data s16 -> u10
	temp = (imu_GyroZ_raw / 64);
	temp = temp * 5 / 4;
	temp = 512 + temp;
	if (temp > 1023 ) temp = 1023;
	if (temp < 0 ) temp = 0;
	imu_GyroZ_raw = temp;

	//dbgu_printf("GYRO: %d, %d, %d\r\n", imu_GyroX_raw, imu_GyroY_raw, imu_GyroZ_raw);
}

void IMU_magnet_init(void) {
	I2C_WriteRegister(HMC_ADDRESS, HMC_CONFIG_A, 0x70); //8 average, 15Hz, normal measurement
	I2C_WriteRegister(HMC_ADDRESS, HMC_CONFIG_B, 0x20); //configuration gain = 1.3
	I2C_WriteRegister(HMC_ADDRESS, HMC_MODE, 0x00); //continuous measurement mode
	TC_DelayMs(10);
}

void IMU_magnet_update(void) {
	unsigned char data[6];
	I2C_ReadMultipleRegisters(HMC_ADDRESS, 0x03, &data[0], 6);

#ifdef DYNAMIXEL
	imu_MagnetZ_raw = (signed short)(data[1] | (data[0] << 8));
	imu_MagnetX_raw = (signed short)(data[3] | (data[2] << 8));
	imu_MagnetY_raw = (signed short)(data[5] | (data[4] << 8));
#else
	imu_MagnetX_raw = (signed short)(data[1] | (data[0] << 8));
	imu_MagnetY_raw = (signed short)(data[3] | (data[2] << 8));
	imu_MagnetZ_raw = (signed short)(data[5] | (data[4] << 8));
#endif
	//dbgu_printf("MAGNET: %d, %d, %d   ", imu_MagnetX_raw, imu_MagnetY_raw, imu_MagnetZ_raw);
}

double IMU_get_temp(void) {
	unsigned char data[2];
	I2C_ReadMultipleRegisters(ITG_ADDRESS, 0x1b, &data[0], 2);

	int raw = (signed short)(data[1] | (data[0] << 8));

	return ((raw + 13200) / 280) + 35;
}

double IMU_CalculateHeadingTiltCompensated(void)
{
	// Convert acceleration to G
	float accX = ((float)(imu_FBAccel - 512)) / 128.0f;
	float accY = ((float)(imu_RLAccel - 512)) / 128.0f;

	float rollRadians = asin(accX);
	float pitchRadians = asin(accY);

	// We cannot correct for tilt over 40 degrees with this algorithm, if the board is tilted as such, return 0.
	if(rollRadians > 0.78 || rollRadians < -0.78 || pitchRadians > 0.78 || pitchRadians < -0.78)
	{
		return 0;
	}

	// Some of these are used twice, so rather than computing them twice in the algorithm we precompute them before hand.
	float cosRoll = cos(rollRadians);
	float sinRoll = sin(rollRadians);
	float cosPitch = cos(pitchRadians);
	float sinPitch = sin(pitchRadians);

	// The tilt compensation algorithm.
	float Xh = imu_UDMagnet * cosPitch + imu_FBMagnet * sinPitch;
	float Yh = imu_UDMagnet * sinRoll * sinPitch + imu_RLMagnet * cosRoll - imu_FBMagnet * sinRoll * cosPitch;

	double heading = atan2(Yh, Xh);

	return heading;
}

void IMU_UpdateStatus(struct imuData_t* data) {
	data->fb_accel = imu_FBAccel;
	data->rl_accel = imu_RLAccel;
	data->fb_gyro = imu_FBGyro;
	data->rl_gyro = imu_RLGyro;
}
