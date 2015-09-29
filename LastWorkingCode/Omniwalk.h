// Módulo de testes

#ifndef _Omniwalk_H_
#define _Omniwalk_H_

#include "MotionPattern.h"
#include "MotionModule.h"
#include "AttitudeEstimation.h"

#define HIP_PITCH_OFFSET 13.0
#define C21 3.5 // Gait Velocity Limiting Norm p
#define C22 0.1 // Sagital Acceleretion
#define C23 0.1 // Lateral Acceleretion
#define C24 0.009 // Rotational Acceleretion
#define C25 0.07 //0.07 // Constant Step Frequency
#define C26 0.008 // Sagital Proportional Step Frequency
#define C27 0.01 // Lateral Proportional Step Frequency
#define VX_MAX 0.05 // Max Sagital Speed
#define VY_MAX 0.05 // Max Lateral Speed
#define VPHI_MAX 0.8 // Max Rotational Speed
#define DT 0.008 // 8 ms

namespace Robot
{
    class Omniwalk
    {
        private:
            static Omniwalk* m_UniqueInstance;
            double m_RFootPose[6];
			double m_LFootPose[6];
			double m_VxTarget,m_VyTarget,m_VphiTarget; // V desired
			double m_Vx,m_Vy,m_Vphi; // V in the algorithm
			double m_Tau, m_TauRate;
			
			void m_LegInterface(double *out, double eta, double roll, double pitch, double yaw);
			void m_ControlInterface();
			
        public:
            static Omniwalk* GetInstance() { return m_UniqueInstance; }
			
            double GetRx(){ return m_RFootPose[0]; }
			double GetRy(){ return m_RFootPose[1]; }
			double GetRz(){ return m_RFootPose[2]; }
			double GetRroll(){ return m_RFootPose[3]; }
			double GetRpitch(){ return m_RFootPose[4]; }
			double GetRyaw(){ return m_RFootPose[5]; }
			double GetLx(){ return m_LFootPose[0]; }
			double GetLy(){ return m_LFootPose[1]; }
			double GetLz(){ return m_LFootPose[2]; }
			double GetLroll(){ return m_LFootPose[3]; }
			double GetLpitch(){ return m_LFootPose[4]; }
			double GetLyaw(){ return m_LFootPose[5]; }
			
			double GetTau(){ return m_Tau; }
			double GetTauRate(){ return m_TauRate; }
			
			void SetTau(double Tau){ m_Tau = Tau; }
			
            void Init();
            void Update(double Vx, double Vy, double Vphi);
    };
}
#endif

