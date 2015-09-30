// Módulo de testes

#ifndef _AttitudeEstimation_H_
#define _AttitudeEstimation_H_

#include "MotionModule.h"
#include "CM730.h"
#include "Kalman.h"

namespace Robot
{
    class AttitudeEstimation:MotionModule
    {
        private:
            static AttitudeEstimation* m_UniqueInstance; 
            AttitudeEstimation();

			double m_Roll,m_Pitch;
			double m_RollAcc,m_PitchAcc;
			double m_RollGyro,m_PitchGyro;
			double m_dt;
			double m_Fallen;
			double m_gx,m_gy,m_gz,m_ax,m_ay,m_az;
			bool m_valid;
			
			double m_AttRollEstimation(double a_y, double a_z);
			double m_AttPitchEstimation(double a_x);
            
			unsigned char m_table[CM730::MAXNUM_ADDRESS];
			CM730* m_Controller;
			Kalman FilterPitch,FilterRoll;			
			 
			bool m_UpdateNavData(void);

			
        public:
            static AttitudeEstimation* GetInstance() { return m_UniqueInstance; }
			~AttitudeEstimation();

            double Get_Roll() {return m_Roll;}
            double Get_Fallen() {return m_Fallen;}
            double Get_Pitch() {return m_Pitch;}
			bool IsValid() {return m_valid;}
			
			void SetController(CM730* address) {m_Controller = address;}
			
			void Initialize();
            void Process();
    };
}
#endif

