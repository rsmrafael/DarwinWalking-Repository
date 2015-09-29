#ifndef MOTIONPATTERN_H

#define LEFT_LEG 1
#define RIGHT_LEG -1
#define TAU0 28
#define TAU1 29

class MotionPattern
{
    public:

        static MotionPattern *m_MotionPattern;

        MotionPattern();
        
        double Active;
        double StartStep;
		double StopStep; 
		
		bool Starting,Stoping;
        void Start(double StartTime, double dt);
        void Stop(double StopTime, double dt);
		double GetActive(){ return Active;}
		

        void getHaltPosition(int leg, double tau,
                double Vx, double Vy, double Vphi, double *eta,
                double *legRoll, double *legPitch, double *legYaw,
                double *footRoll, double *footPitch);

        void getLegLifting(int leg, double tau,
                double Vx, double Vy, double Vphi, double *eta,
                double *legRoll, double *legPitch, double *legYaw,
                double *footRoll, double *footPitch);

        void getLegSwing(int leg, double tau,
                double Vx, double Vy, double Vphi, double *eta,
                double *legRoll, double *legPitch, double *legYaw,
                double *footRoll, double *footPitch);

        void getLateralHipSwing(int leg, double tau,
                double Vx, double Vy, double Vphi, double *eta,
                double *legRoll, double *legPitch, double *legYaw,
                double *footRoll, double *footPitch);

        void getLeaning(int leg, double tau,
                double Vx, double Vy, double Vphi, double *eta,
                double *legRoll, double *legPitch, double *legYaw,
                double *footRoll, double *footPitch);

        static MotionPattern *getInstance() { return m_MotionPattern; }

        //
        // Returns the position of the leg in the parametric space
        // using spherical coordinates: eta, roll and pitch
        //
        // Parameters:
        // leg -- LEFT_LEG or RIGHT_LEG
        // tau -- phase of the movement
        //
        void getLeg(int leg, double tau,
                double Vx, double Vy, double Vphi, double *eta,
                double *legRoll, double *legPitch, double *legYaw,
                double *footRoll, double *footPitch);

        //
        // These are the parameters that define the movement
        //
        double m_C[30];
};

#define MOTIONPATTERN_H
#endif

