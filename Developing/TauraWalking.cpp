/*
 *   TauraWalking.cpp
 *
 *   Author: Rafael
 *
 */

#include <cstdio>
#include <cmath> // ????

#include "MX28.h" // Constant of the motors
//#include "CM730.h" // ?????
#include "Vector.h" // used in ComputeIK
#include "Matrix.h" // used in ComputeIK
#include "Kinematics.h" // Robot structure constants

#include "AttitudeEstimation.h"
#include "Omniwalk.h"
#include "TauraWalking.h"

#define SQR(x) ((x)*(x))
#define MARGIN M_PI/18.0
#define LEG_MARGIN 5 //mm

using namespace Robot;

TauraWalking* TauraWalking::m_UniqueInstance = new TauraWalking();

bool TauraWalking::m_computeIK(double *out, double x, double y, double z, double a, double b, double c)
{
	Matrix3D Tad, Tda, Tcd, Tdc, Tac;
	Vector3D vec;
    double _Rac, _Acos, _Atan, _k, _l, _m, _n, _s, _c, _theta;
	double LEG_LENGTH = Kinematics::LEG_LENGTH;
	double THIGH_LENGTH = Kinematics::THIGH_LENGTH;
	double CALF_LENGTH = Kinematics::CALF_LENGTH;
	double ANKLE_LENGTH = Kinematics::ANKLE_LENGTH;
	
	Tad.SetTransform(Point3D(x, y, z - LEG_LENGTH), Vector3D(a * 180.0 / M_PI, b * 180.0 / M_PI, c * 180.0 / M_PI));

	vec.X = x + Tad.m[2] * ANKLE_LENGTH;
    vec.Y = y + Tad.m[6] * ANKLE_LENGTH;
    vec.Z = (z - LEG_LENGTH) + Tad.m[10] * ANKLE_LENGTH;

    // Get Knee
	_Rac = vec.Length();
    _Acos = acos((_Rac * _Rac - THIGH_LENGTH * THIGH_LENGTH - CALF_LENGTH * CALF_LENGTH) / (2 * THIGH_LENGTH * CALF_LENGTH));
    if(isnan(_Acos) == 1)
		return false;
    *(out + 3) = _Acos;

    // Get Ankle Roll
    Tda = Tad;
	if(Tda.Inverse() == false)
        return false;
    _k = sqrt(Tda.m[7] * Tda.m[7] + Tda.m[11] * Tda.m[11]);
    _l = sqrt(Tda.m[7] * Tda.m[7] + (Tda.m[11] - ANKLE_LENGTH) * (Tda.m[11] - ANKLE_LENGTH));
    _m = (_k * _k - _l * _l - ANKLE_LENGTH * ANKLE_LENGTH) / (2 * _l * ANKLE_LENGTH);
    if(_m > 1.0)
        _m = 1.0;
    else if(_m < -1.0)
        _m = -1.0;
    _Acos = acos(_m);
    if(isnan(_Acos) == 1)
        return false;
    if(Tda.m[7] < 0.0)
        *(out + 5) = -_Acos;
    else
        *(out + 5) = _Acos;

    // Get Hip Yaw
	Tcd.SetTransform(Point3D(0, 0, -ANKLE_LENGTH), Vector3D(*(out + 5) * 180.0 / M_PI, 0, 0));
	Tdc = Tcd;
	if(Tdc.Inverse() == false)
        return false;
	Tac = Tad * Tdc;
    _Atan = atan2(-Tac.m[1] , Tac.m[5]);
    if(isinf(_Atan) == 1)
        return false;
    *(out) = _Atan;

    // Get Hip Roll
    _Atan = atan2(Tac.m[9], -Tac.m[1] * sin(*(out)) + Tac.m[5] * cos(*(out)));
    if(isinf(_Atan) == 1)
        return false;
    *(out + 1) = _Atan;

    // Get Hip Pitch and Ankle Pitch
    _Atan = atan2(Tac.m[2] * cos(*(out)) + Tac.m[6] * sin(*(out)), Tac.m[0] * cos(*(out)) + Tac.m[4] * sin(*(out)));
    if(isinf(_Atan) == 1)
        return false;
    _theta = _Atan;
    _k = sin(*(out + 3)) * CALF_LENGTH;
    _l = -THIGH_LENGTH - cos(*(out + 3)) * CALF_LENGTH;
	_m = cos(*(out)) * vec.X + sin(*(out)) * vec.Y;
	_n = cos(*(out + 1)) * vec.Z + sin(*(out)) * sin(*(out + 1)) * vec.X - cos(*(out)) * sin(*(out + 1)) * vec.Y;
    _s = (_k * _n + _l * _m) / (_k * _k + _l * _l);
    _c = (_n - _k * _s) / _l;
    _Atan = atan2(_s, _c);
    if(isinf(_Atan) == 1)
        return false;
    *(out + 2) = _Atan;
    *(out + 4) = _theta - *(out + 3) - *(out + 2);

    //Correction for HIP ROLL
	*(out + 1) = (*(out + 5)-a) * -1;

    return true;
}

void TauraWalking::Set_V_Target(double x, double y, double phi)
{
	// Set desired velocity
	m_VxTarget=-x;
	m_VyTarget=-y;
	m_VphiTarget=-phi;
} 

void TauraWalking::Start(double StartTime, double dt){
    m_Starting=true;
    m_Stoping=false;
	m_StartStep = dt/StartTime;
}

void TauraWalking::Stop(double StopTime, double dt){
    m_Stoping=true;
    m_Starting=false;
	m_StopStep = dt/StopTime;
}

void TauraWalking::Initialize()
{
	Omniwalk::GetInstance()->Init();
	
	m_VxTarget = 0;
	m_VyTarget = 0;
	m_VphiTarget = 0;

	// Feet positions and orientations
	m_Rx = -10;
	m_Ry = Kinematics::LEG_SIDE_OFFSET+2.5;
	m_Rz = Kinematics::LEG_LENGTH-20;
	m_Rroll = 0;
	m_Rpitch = 0;
	m_Ryaw = 0;
	m_Lx = -10;
	m_Ly = -2.5-Kinematics::LEG_SIDE_OFFSET;
	m_Lz = Kinematics::LEG_LENGTH-20;
	m_Lroll = 0;
	m_Lpitch = 0;
	m_Lyaw = 0;
	
	m_Active = 0;
	m_Starting = false;
	m_Stoping = false;
	Omniwalk::GetInstance()->getHaltPosition(RIGHT_LEG , m_RHaltPosition);
	Omniwalk::GetInstance()->getHaltPosition(LEFT_LEG , m_LHaltPosition);
	
}

void TauraWalking::Process()
{
	enum WalkingPhase{
		LEFTSUPPORT,
		L2R,
		RIGHTSUPPORT,
		R2L};
	WalkingPhase phase;
	
	double Tau;
	Tau = Omniwalk::GetInstance()->GetTau();
	
	// Phase detection
	if((Tau>=(MARGIN-M_PI))&&(Tau<-MARGIN))	phase = RIGHTSUPPORT;
	else if ((Tau>=MARGIN) && (Tau<(M_PI-MARGIN))) phase = LEFTSUPPORT;
	else{ //double support
		if((Tau>=-MARGIN)&&(Tau<MARGIN))  phase = R2L;
		else phase = L2R;
	}
		
    if(AttitudeEstimation::GetInstance()->IsValid())
	{  // Lowest foot detection  
        double Roll = AttitudeEstimation::GetInstance()->Get_Roll();
        double Pitch = AttitudeEstimation::GetInstance()->Get_Pitch();
      
        double RealLeftZ = -sin(Pitch)*m_Lx+sin(Roll)*cos(Pitch)*(m_Ly-Kinematics::LEG_SIDE_OFFSET)+cos(Roll)*cos(Pitch)*m_Lz;
        double RealRightZ = -sin(Pitch)*m_Rx+sin(Roll)*cos(Pitch)*m_Ry+cos(Roll)*cos(Pitch)*m_Rz;
		
		// If haven't reach floor, wait
        if((RealLeftZ+LEG_MARGIN>RealRightZ)&&(phase==RIGHTSUPPORT)){
            Omniwalk::GetInstance()->SetTau(M_PI-MARGIN);
            phase = LEFTSUPPORT;
        }
        else if((RealRightZ+LEG_MARGIN>RealLeftZ)&&(phase==LEFTSUPPORT)){
            Omniwalk::GetInstance()->SetTau(-MARGIN);
            phase = RIGHTSUPPORT;
        }
    }
	
	Omniwalk::GetInstance()->Update(m_VxTarget,m_VyTarget,m_VphiTarget);
	
	if(m_Starting){
        if(m_Active>=1){
            m_Active=1;
            m_Starting=false;
        }
        else m_Active += m_StartStep;
    }

    if(m_Stoping){
        if(m_Active<=0){
            m_Active=0;
            m_Stoping=false;
        }
        else m_Active -= m_StopStep;
    }
	
	// Desired position
    m_Rx = m_RHaltPosition[0]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetRx();
    m_Ry = m_RHaltPosition[1]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetRy();
	m_Rz = m_RHaltPosition[2]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetRz();
	m_Rroll = m_RHaltPosition[3]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetRroll();
	m_Rpitch = m_RHaltPosition[4]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetRpitch();
	m_Ryaw = m_RHaltPosition[5]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetRyaw();
    m_Lx = m_LHaltPosition[0]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetLx();
    m_Ly = m_LHaltPosition[1]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetLy();
	m_Lz = m_LHaltPosition[2]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetLz();
	m_Lroll = m_LHaltPosition[3]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetLroll();
	m_Lpitch = m_LHaltPosition[4]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetLpitch();
	m_Lyaw = m_LHaltPosition[5]*(1-m_Active) + m_Active * Omniwalk::GetInstance()->GetLyaw();

    //printf("Rx:%f\tRy:%f\tRz:%f\nLx:%f\tLy:%f\tLz:%f\n",m_Rx,m_Ry,m_Rz,m_Lx,m_Ly,m_Lz);

    // Saturation to avoid colision in Y axis
	if(m_Ry<(25-Kinematics::LEG_SIDE_OFFSET/2)) m_Ry=25-Kinematics::LEG_SIDE_OFFSET/2;
	if(m_Ly>(-25+Kinematics::LEG_SIDE_OFFSET/2)) m_Ly=-25+Kinematics::LEG_SIDE_OFFSET/2;
	
	// Saturation to reach desired orientation
	double aux; // auxiliary variable
	aux = SQR(m_Rx)+SQR(m_Ry)+SQR(m_Rz);  // SQR(Desired Length)
	if(aux>0.97*SQR(Kinematics::LEG_LENGTH)){
		aux=0.97*Kinematics::LEG_LENGTH/sqrt(aux); // Rate between desired position and Leg_lenght
		m_Rx*=aux;
		m_Ry*=aux;
		m_Rz*=aux;
	}
	aux = SQR(m_Lx)+SQR(m_Ly)+SQR(m_Lz);
	if(aux>0.97*SQR(Kinematics::LEG_LENGTH)){
		aux=0.97*Kinematics::LEG_LENGTH/sqrt(aux); // Rate between desired position and Leg_lenght
		m_Lx*=aux;
		m_Ly*=aux;
		m_Lz*=aux;
	}    

	// Compute IK with reference point rotated and in the hip
	if((m_computeIK(m_RAngles, m_Rx, -m_Ry, Kinematics::LEG_LENGTH-m_Rz, m_Rroll, m_Rpitch, m_Ryaw) == 1)
	&& (m_computeIK(m_LAngles, m_Lx, -m_Ly, Kinematics::LEG_LENGTH-m_Lz, m_Lroll, m_Lpitch, m_Lyaw) == 1))
		{
		
		int dir[12]          = {   -1,        -1,          1,         1,         -1,            1,          -1,        -1,         -1,         -1,         1,            1};
		double initAngle[12] = {   0.0,       0.0,        0.0,       0.0,        0.0,          0.0,         0.0,       0.0,        0.0,        0.0,       0.0,          0.0};

		for(int i=0; i<=5; i++)
		{
			m_RAngles[i] *= 180.0 / M_PI;
			m_LAngles[i] *= 180.0 / M_PI;
		}
		// Compute motor value
        for(int i=0; i<=5; i++)
        {
            double offset = (double)dir[i] * m_RAngles[i] * MX28::RATIO_ANGLE2VALUE;
            if(i == 2) // R_HIP_PITCH
                offset -= (double)dir[2] * HIP_PITCH_OFFSET * MX28::RATIO_ANGLE2VALUE;

            m_RAngles[i] = MX28::Angle2Value(initAngle[i]) + (int)offset;
        }

        for(int i=0; i<=5; i++)
        {
            double offset = (double)dir[i+6] * m_LAngles[i] * MX28::RATIO_ANGLE2VALUE;
            if(i == 2) // L_HIP_PITCH
                offset -= (double)dir[8] * HIP_PITCH_OFFSET * MX28::RATIO_ANGLE2VALUE;

            m_LAngles[i] = MX28::Angle2Value(initAngle[i+6]) + (int)offset;
        }
		
		
		if(m_Joint.GetEnable(JointData::ID_R_HIP_YAW) == true)
			m_Joint.SetValue(JointData::ID_R_HIP_YAW,           m_RAngles[0]);
		if(m_Joint.GetEnable(JointData::ID_R_HIP_ROLL) == true)
			m_Joint.SetValue(JointData::ID_R_HIP_ROLL,          m_RAngles[1]);
		if(m_Joint.GetEnable(JointData::ID_R_HIP_PITCH) == true)
			m_Joint.SetValue(JointData::ID_R_HIP_PITCH,         m_RAngles[2]);
		if(m_Joint.GetEnable(JointData::ID_R_KNEE) == true)
			m_Joint.SetValue(JointData::ID_R_KNEE,              m_RAngles[3]);
		if(m_Joint.GetEnable(JointData::ID_R_ANKLE_PITCH) == true)
			m_Joint.SetValue(JointData::ID_R_ANKLE_PITCH,       m_RAngles[4]);
		if(m_Joint.GetEnable(JointData::ID_R_ANKLE_ROLL) == true)
			m_Joint.SetValue(JointData::ID_R_ANKLE_ROLL,        m_RAngles[5]);
		if(m_Joint.GetEnable(JointData::ID_L_HIP_YAW) == true)
			m_Joint.SetValue(JointData::ID_L_HIP_YAW,           m_LAngles[0]);
		if(m_Joint.GetEnable(JointData::ID_L_HIP_ROLL) == true)
			m_Joint.SetValue(JointData::ID_L_HIP_ROLL,          m_LAngles[1]);
		if(m_Joint.GetEnable(JointData::ID_L_HIP_PITCH) == true)
			m_Joint.SetValue(JointData::ID_L_HIP_PITCH,         m_LAngles[2]);
		if(m_Joint.GetEnable(JointData::ID_L_KNEE) == true)	
			m_Joint.SetValue(JointData::ID_L_KNEE,              m_LAngles[3]);
		if(m_Joint.GetEnable(JointData::ID_L_ANKLE_PITCH) == true)
			m_Joint.SetValue(JointData::ID_L_ANKLE_PITCH,       m_LAngles[4]);
		if(m_Joint.GetEnable(JointData::ID_L_ANKLE_ROLL) == true)
			m_Joint.SetValue(JointData::ID_L_ANKLE_ROLL,        m_LAngles[5]);

        }
    else printf("Fail in the IK\n");
}
