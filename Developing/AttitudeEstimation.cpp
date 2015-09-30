//qualquer modificação aleatória pra dar pull


#include <cmath>
#include <cstdio>

#include "AttitudeEstimation.h"
#include "CM730.h"

#define ACCTHRESHOLD 0.25            // Consider acc if Norm(ay||ax,az)>AccThreshold*norm(ax,ay,az)
#define PITCHOFFSET -0.26           // about -0.3 in initial tests
#define ROLLOFFSET 0.05				// Very low between -0.05 and 0.05
#define COMPLEMENTARYFILTER 0.1     // 0 - only gyro | 1 - only Acc
#define SAGITALFALLEN 0.9           // Accelerometer value when fallen in sagital direction
#define LATERALFALLEN 0.85          // Accelerometer value when fallen in lateral direction


#define SQR(x) ((x)*(x))

using namespace Robot;

AttitudeEstimation *AttitudeEstimation::m_UniqueInstance = new AttitudeEstimation();

AttitudeEstimation::AttitudeEstimation(){
}

AttitudeEstimation::~AttitudeEstimation(){ 
}

//roll angle from acc in radians
double AttitudeEstimation::m_AttRollEstimation(double a_y, double a_z)
{
	return atan2(a_y, a_z);
}

//pitch angle from acc in radians
double AttitudeEstimation::m_AttPitchEstimation(double a_x)
{
	if (a_x>=1) return M_PI/2;
	else if (a_x<=-1) return -M_PI/2;
	else return asin(a_x);
}

bool AttitudeEstimation::m_UpdateNavData() // Read Accs and Gyros
{
	int result=0;
	int error=0;
    
    if(m_Controller!=0){		
        result = m_Controller->ReadTable(CM730::ID_CM, CM730::P_GYRO_Z_L, CM730::P_ACCEL_Z_H, m_table, &error);

	    if (result == CM730::SUCCESS)
	    {
		    double gtemp[3],atemp[3];	
		    gtemp[0] = double(((unsigned short)m_table[CM730::P_GYRO_X_H] << 8) | m_table[CM730::P_GYRO_X_L]);
		    gtemp[1] = double(((unsigned short)m_table[CM730::P_GYRO_Y_H] << 8) | m_table[CM730::P_GYRO_Y_L]);
		    gtemp[2] = double(((unsigned short)m_table[CM730::P_GYRO_Z_H] << 8) | m_table[CM730::P_GYRO_Z_L]);
		    atemp[0] = double(((unsigned short)m_table[CM730::P_ACCEL_X_H] << 8) | m_table[CM730::P_ACCEL_X_L]);
		    atemp[1] = double(((unsigned short)m_table[CM730::P_ACCEL_Y_H] << 8) | m_table[CM730::P_ACCEL_Y_L]);
		    atemp[2] = double(((unsigned short)m_table[CM730::P_ACCEL_Z_H] << 8) | m_table[CM730::P_ACCEL_Z_L]);
	
            // Gyroscope reading rad/s
            m_gx = -(gtemp[0] - 512.0)*3.125*M_PI/180.0; 
            m_gy = (gtemp[1] - 512.0)*3.125*M_PI/180.0;
            m_gz = -(gtemp[2] - 512.0)*3.125*M_PI/180.0;
		    // Accelerometer reading
            m_ax = (atemp[1] - 512.0) / 128.0; // 1 = 1g = 9.81 m/s^2
		    m_ay = -(atemp[0] - 512.0) / 128.0;
		    m_az = (atemp[2] - 512.0) / 128.0;
            //printf("ax:%f\tay:%f\taz:%f\ngx:%f\tgy:%f\tgz:%f\n",m_ax,m_ay,m_az,m_gx,m_gy,m_gz);
		    return true;
	    }
	    else
	    {
		    m_gx=0;
		    m_gy=0;
		    m_gz=0;
		    m_ax = 0;
		    m_ay = 0;
		    m_az = 0;
	    }
    }
    printf("NavData cannot be read - Controller address: %p\n",m_Controller);
	return false;
}

void AttitudeEstimation::Initialize(){
	m_Roll = 0;
	m_Pitch = 0;
	m_RollAcc = 0;
	m_PitchAcc = 0;
	m_RollGyro = 0;
	m_PitchGyro = 0;
	m_gx = 0;
	m_gy = 0;
	m_gz = 0;
	m_ax = 0;
	m_ay = 0;
	m_az = 1;
	m_dt=8.0/1000.0; // conferir
	m_valid = false;
    m_Fallen = 0; // stand up
    for(int i=0; i<CM730::MAXNUM_ADDRESS; i++)	m_table[i] = 0;

}

void AttitudeEstimation::Process(){
	m_valid = m_UpdateNavData(); //update Accs and Gyro readings
	//printf("Valid: %d\n",valid);
    if (m_valid){ 
		
		// --------------- attitude estimation ----------------------------
		if ((SQR(m_ay)+SQR(m_az))<(SQR(ACCTHRESHOLD)*(SQR(m_ax)+SQR(m_ay)+SQR(m_az)))){
		    if (m_ax>=SAGITALFALLEN) m_Fallen = -1; // back fall
            else if (m_ax<=-SAGITALFALLEN) m_Fallen = 1; // front fall
        }
        else if ((SQR(m_ax)+SQR(m_az))<(SQR(ACCTHRESHOLD)*(SQR(m_ax)*SQR(m_ay)+SQR(m_az)))){
            if (m_ay>=LATERALFALLEN) m_Fallen = -2; // left fall
            else if (m_ay<=-LATERALFALLEN) m_Fallen = 2; // right fall
        }
        else m_Fallen = 0;

		if (!m_Fallen){ // ay and az both have meaningful value (at least one >> 0)
			m_RollAcc  = m_AttRollEstimation(m_ay,m_az)-ROLLOFFSET; // atan2(ay,az) - offset
			m_RollGyro = m_Roll + (m_gx+m_gy*sin(m_Roll)*tan(m_Pitch)+m_gz*cos(m_Roll)*tan(m_Pitch))*m_dt; // Estimated Roll
			m_Roll = m_RollGyro+(m_RollAcc-m_RollGyro)*COMPLEMENTARYFILTER; // fixed-gain observer
			
		}
		else m_Roll += (m_gx+m_gy*sin(m_Roll)*tan(m_Pitch)+m_gz*cos(m_Roll)*tan(m_Pitch))*m_dt; // Accelerometer measurement not reliable for Roll angle

		if (!m_Fallen){
			m_PitchAcc = m_AttPitchEstimation(m_ax)-PITCHOFFSET; //asin(ax)
			m_PitchGyro = m_Pitch + (m_gy*cos(m_Roll)-m_gz*sin(m_Roll))*m_dt; // Estimated Pitch
			m_Pitch = m_PitchGyro+(m_PitchAcc-m_PitchGyro)*COMPLEMENTARYFILTER; // fixed-gain observer

		}  
		else m_Pitch += (m_gy*cos(m_Roll)-m_gz*sin(m_Roll))*m_dt; // Accelerometer measurement not reliable for Pitch angle
		
	    //printf("Roll:\t%.2f\tPitch:\t%.2f\n", Roll,Pitch );
	}

}
