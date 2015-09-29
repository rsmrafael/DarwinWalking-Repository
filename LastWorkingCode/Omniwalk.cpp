/*
 *   Omniwalk.cpp
 *
 *   Author: Rafael
 *
 */


#include <cstdio>
#include <cmath>
#include <algorithm> //fmin e fmax

#include "Kinematics.h"
#include "MotionPattern.h"
#include "AttitudeEstimation.h"
#include "Omniwalk.h"

#define SQR(x) ((x)*(x))
#define MARGIN M_PI/18.0

using namespace Robot;

Omniwalk* Omniwalk::m_UniqueInstance = new Omniwalk();

void Omniwalk::m_ControlInterface()
{

    double Vx_norm,Vy_norm,Vphi_norm;
	
	// Linear Mapping
	Vx_norm = m_VxTarget/VX_MAX;
	Vy_norm = m_VyTarget/VY_MAX;
	Vphi_norm = m_VphiTarget/VPHI_MAX;
	
	// P-norm
	double P_norm;
	P_norm = pow((pow(fabs(Vx_norm),C21) + pow(fabs(Vy_norm),C21) + pow(fabs(Vphi_norm),C21)),(double)(1/C21));
    if (P_norm > 1){
		Vx_norm = Vx_norm/P_norm;
		Vy_norm = Vy_norm/P_norm;
		Vphi_norm = Vphi_norm/P_norm;
	}
	
	// Update Velocity
	m_Vx += fmax(-C22,fmin((Vx_norm-m_Vx),C22));
	m_Vy += fmax(-C23,fmin((Vy_norm-m_Vy),C23));
	m_Vphi += fmax(-C24,fmin((Vphi_norm-m_Vphi),C24));
	
	// Update Motion Phase
	m_TauRate = (C25 + fabs(m_Vx)*C26 + fabs(m_Vy)*C27);
	m_Tau += m_TauRate;
        if (m_Tau > M_PI) m_Tau -= 2*M_PI;
}

void Omniwalk::m_LegInterface(double *out, double eta, double roll, double pitch, double yaw)
{
	// change from spheric coordinate to cartesian
	double z = (1-eta)*Kinematics::LEG_LENGTH*0.99;
	out[0] = z * (cos(yaw)*sin(pitch)*cos(roll)+sin(yaw)*sin(roll));
	out[1] = z * (sin(yaw)*sin(pitch)*cos(roll)-cos(yaw)*sin(roll));
	out[2] = z * (cos(pitch)*cos(roll));
}

void Omniwalk::Init()
{
	m_Vx = 0;
	m_Vy = 0;
	m_Vphi = 0;
	
	m_VxTarget = 0;
	m_VyTarget = 0;
	m_VphiTarget = 0;
	
	m_Tau = 0;
	m_TauRate = 0;

	m_RFootPose[0] = -10; // x
	m_RFootPose[1] = Kinematics::LEG_SIDE_OFFSET+2.5; // y
	m_RFootPose[2] = Kinematics::LEG_LENGTH-20; // z
	m_RFootPose[3] = 0; // roll
	m_RFootPose[4] = 0; // pitch
	m_RFootPose[5] = 0; // yaw
	m_LFootPose[0] = -10; // x
	m_LFootPose[1] = -2.5-Kinematics::LEG_SIDE_OFFSET; // y
	m_LFootPose[2] = Kinematics::LEG_LENGTH-20; // z
	m_LFootPose[3] = 0; // roll
	m_LFootPose[4] = 0; // pitch
	m_LFootPose[5] = 0; // yaw
}

void Omniwalk::Update(double Vx,double Vy,double Vphi)
{
	m_VxTarget = Vx;
	m_VyTarget = Vy;
	m_VphiTarget = Vphi;
	
	// Limit Velocity 
	m_ControlInterface();
	
	// Motion Patern
	double R_Eta, R_Leg_Roll, R_Leg_Pitch, R_Leg_Yaw, R_Foot_Pitch, R_Foot_Roll;
	double L_Eta, L_Leg_Roll, L_Leg_Pitch, L_Leg_Yaw, L_Foot_Pitch, L_Foot_Roll;
	
    MotionPattern::getInstance()->getLeg(RIGHT_LEG, m_Tau, m_Vx, m_Vy, m_Vphi,
            &R_Eta, &R_Leg_Roll, &R_Leg_Pitch, &R_Leg_Yaw,
            &R_Foot_Roll, &R_Foot_Pitch);
    MotionPattern::getInstance()->getLeg(LEFT_LEG, m_Tau, m_Vx, m_Vy, m_Vphi,
            &L_Eta, &L_Leg_Roll, &L_Leg_Pitch, &L_Leg_Yaw,
            &L_Foot_Roll, &L_Foot_Pitch);
	
	// Change from spheric coordinates to cartesian (Leg Interface)
	m_LegInterface(m_RFootPose,R_Eta,R_Leg_Roll,R_Leg_Pitch,R_Leg_Yaw);
	m_RFootPose[3] = R_Foot_Roll;
	m_RFootPose[4] = R_Foot_Pitch;
	m_RFootPose[5] = R_Leg_Yaw;
	
	m_LegInterface(m_LFootPose,L_Eta,L_Leg_Roll,L_Leg_Pitch,L_Leg_Yaw);
	m_LFootPose[3] = L_Foot_Roll;
	m_LFootPose[4] = L_Foot_Pitch;
	m_LFootPose[5] = L_Leg_Yaw;
}
