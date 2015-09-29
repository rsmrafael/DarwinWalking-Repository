// Módulo de testes

#ifndef _CAPTURESTEP_H_
#define _CAPTURESTEP_H_

#include "ars.h"


    class CaptureStep
    {
        private:
            static CaptureStep* m_UniqueInstance; 
            CaptureStep();

			double CoM[2]; // Centro de Massa Extrapolado
			double dCoMdt[2]; // Velocidade do Centro de Massa Extrapolado
			double XCoM[2];
			double T; // Tempo entre passada
			double W0;
			double dt;
			double Ux, Uy;
			double Vx, Vy;
			
			double calculateCOM(int axis, double Roll, double Pitch, double X, double Y, double Z);
			
			 
        public:
            static CaptureStep* GetInstance() { return m_UniqueInstance; }
			~CaptureStep();
			
			void update(double LegX, double LegY, double LegZ, double eta, char SuportLeg); // update CoM, dCoMdt, XCoM, Ux and Uy
			double Get_Ux() {return Ux;}
			double Get_Uy() {return Uy;}
			void Set_Vx(double value) {Vx = value;}
            void Set_Vy(double value) {Vy = value;}
    };

#endif

