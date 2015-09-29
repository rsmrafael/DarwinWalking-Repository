// Módulo de testes

#ifndef _TAURAWALKING_H_
#define _TAURAWALKING_H_

#include "MotionModule.h"

namespace Robot
{
    class TauraWalking : MotionModule
    {
        private:
            static TauraWalking* m_UniqueInstance;
			double m_VxTarget,m_VyTarget,m_VphiTarget; // V for TauraWalking algorithm
            double m_Rx,m_Ry,m_Rz,m_Rroll,m_Rpitch,m_Ryaw,m_Lx,m_Ly,m_Lz,m_Lroll,m_Lpitch,m_Lyaw;
            double m_RAngles[6];
			double m_LAngles[6];
			
			double m_Active;
			bool m_Starting;
			bool m_Stoping;
			double m_StartStep;
			double m_StopStep; 
			double m_RHaltPosition[6];
			double m_LHaltPosition[6];
			
			bool m_computeIK(double *out, double x, double y, double z, double a, double b, double c);
			
        public:
            static TauraWalking* GetInstance() { return m_UniqueInstance; }
        
            void Set_V_Target(double x, double y, double phi); 
			double Get_Vx(void) { return m_VxTarget; }
			double Get_Vy(void) { return m_VyTarget; }
			double Get_Vphi(void) { return m_VphiTarget; }
			
			
			void Start(double StartTime, double dt);
			void Stop(double StopTime, double dt);
			double GetActive(){ return Active;}
			
            void Initialize();
            void Process();
    };
}
#endif

