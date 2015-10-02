#include "Walking.h"
#include "MotionStatus.h"
#include "MX28.h"
#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace std;
using namespace Robot;

int main(int argc, char *argv[])
{
    if (argc != 13) {
        cout << "Usage:" << argv[0] << " ep0 ep1 ep2 ep3 ... ep11" << endl;
    } else {
    	double ep[12];
        double angle[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        double initAngle[14] = {0,0,0,0,0,0,0,0,0,0,0,0,-48.345,41.313};

        for (int i = 0 ; i < 12 ; i++)
        {
            ep[i] = atof(argv[i]);
        }

        bool b1 = Walking::GetInstance()->computeIK(angle,ep[0],ep[1],
                                                 ep[2],ep[3],ep[4],ep[5]);
        bool b2 = Walking::GetInstance()->computeIK(angle+6,ep[6],ep[7],
                                                 ep[8],ep[9],ep[10],ep[11]);
        if (b1 == 1 && b2 == 1)
        {
            for (int i = 0 ; i < 12 ; i++) angle[i] *= 180 / M_PI;
        }

        int dir[14] = {-1,-1, 1, 1,-1, 1,-1,-1,-1,-1, 1, 1, 1,-1};

        int outValue[14];
        for(int i=0; i<14; i++)
        {
            double offset = (double)dir[i] * angle[i] * MX28::RATIO_ANGLE2VALUE;
            if(i == 2 || i == 8) // R_HIP_PITCH or L_HIP_PITCH
                offset -= (double)dir[i] * Walking::GetInstance()->HIP_PITCH_OFFSET * MX28::RATIO_ANGLE2VALUE;

            outValue[i] = MX28::Angle2Value(initAngle[i]) + (int)offset;

            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_R_HIP_YAW,           outValue[0]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_R_HIP_ROLL,          outValue[1]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_R_HIP_PITCH,         outValue[2]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_R_KNEE,              outValue[3]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_R_ANKLE_PITCH,       outValue[4]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_R_ANKLE_ROLL,        outValue[5]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_L_HIP_YAW,           outValue[6]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_L_HIP_ROLL,          outValue[7]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_L_HIP_PITCH,         outValue[8]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_L_KNEE,              outValue[9]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_L_ANKLE_PITCH,       outValue[10]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_L_ANKLE_ROLL,        outValue[11]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_R_SHOULDER_PITCH,    outValue[12]);
            Walking::GetInstance()->m_Joint.SetValue(JointData::ID_L_SHOULDER_PITCH,    outValue[13]);
            Walking::GetInstance()->m_Joint.SetAngle(JointData::ID_HEAD_PAN, Walking::GetInstance()->A_MOVE_AMPLITUDE);
        }   
    }

    return 0;
}

