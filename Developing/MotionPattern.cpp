#include "MotionPattern.h"
#include <cmath>

MotionPattern *MotionPattern::m_MotionPattern = new MotionPattern();

MotionPattern::MotionPattern()
{
    m_C[1] = 0.03;      // Halt Position Leg Extension
    m_C[2] = 0.18;       // Halt Position Leg Roll Angle
    m_C[3] = 0.035; //0.045;      // Halt Position Leg Pitch Angle
    m_C[4] = 0.03;      // Halt Position Foot Roll Angle
    m_C[5] = 0.0;       // Halt Position Foot Pitch Angle
    m_C[6] = 0.0; //0.02;      // Constant Ground Push (must not be more than m_C[1])
    m_C[7] = 0.0;       // Proportional Ground Push
    m_C[8] = 0.15;     // Constant Step Height
    m_C[9] = 0.11;  //0.12    // Proportional Step Height
    m_C[TAU0] = 0.0;    // Swing Start Timing
    m_C[TAU1] = 2.3876; // Swing Stop Timing
    m_C[10] = 0.17;     // Sagittal Swing Amplitude Fwd
    m_C[11] = 0.12;     // Sagittal Swing Amplitude Bwd
    m_C[12] = 0.12;      // Lateral Swing Amplitude
    m_C[13] = 0.05;     // Lateral Swing Amplitude Offset
    m_C[14] = 0.015;    // Turning Lateral Swing Amplitude Offset
    m_C[15] = 0.2;      // Rotational Swing Amplitude
    m_C[16] = 0.05;     // Rotational Swing Amplitude Offset
    m_C[17] = 0.08;    // Lateral Hip Swing Amplitude
    m_C[18] = 0.0;      // Forward Lean
    m_C[19] = 0.0;      // Backward Lean
    m_C[20] = -0.07;    // Forward and Turning Lean
    m_C[21] = 3.5;      // Gait Velocity Limiting Norm p
    m_C[22] = 0.01;   // Sagittal Acceleration
    m_C[23] = 0.014;     // Lateral Acceleration
    m_C[24] = 0.009;    // Rotational Acceleration
    m_C[25] = 0.09; // 0.09;     // Constant Step Frequency
    m_C[26] = 0.008;    // Sagittal Proportional Step Frequency
    m_C[27] = 0.0;      // Lateral Proportional Step Frequency
}

void MotionPattern::getLeg(int leg, double tau,
        double Vx, double Vy, double Vphi, double *eta,
        double *legRoll, double *legPitch, double *legYaw,
        double *footRoll, double *footPitch)
{
    // Initialize the parameters
    *eta = 0.0;
    *legRoll = 0.0;
    *legPitch = 0.0;
    *legYaw = 0.0;
    *footRoll = 0.0;
    *footPitch = 0.0;
	
	double tau2 = tau;
	if(leg==LEFT_LEG){
		tau2+= M_PI;
		if (tau2 > M_PI) tau2 -= 2*M_PI;
	}

    // Add the halt position components
    getHaltPosition(leg, tau2, Vx, Vy, Vphi, eta,
            legRoll, legPitch, legYaw, footRoll, footPitch);
    // Add the leg liftingh
    getLegLifting(leg, tau2, Vx, Vy, Vphi, eta, legRoll,
            legPitch, legYaw, footRoll, footPitch);
    // Add the leg swing
    getLegSwing(leg, tau2, Vx, Vy, Vphi, eta,
            legRoll, legPitch, legYaw, footRoll, footPitch);
    // Add the lateral hip swing
    getLateralHipSwing(leg, tau, Vx, Vy, Vphi, eta,
            legRoll, legPitch, legYaw, footRoll, footPitch);
    // Add the leaning
    getLeaning(leg, tau2, Vx, Vy, Vphi, eta,
            legRoll, legPitch, legYaw, footRoll, footPitch);

}

void MotionPattern::getHaltPosition(int leg, double tau,
        double Vx, double Vy, double Vphi, double *eta,
        double *legRoll, double *legPitch, double *legYaw,
        double *footRoll, double *footPitch)
{
    *eta += m_C[1];
    *legRoll += (double)leg*m_C[2];
    *legPitch += m_C[3];
    *footRoll += m_C[4];
    *footPitch += m_C[5];
}

void MotionPattern::getLegLifting(int leg, double tau,
        double Vx, double Vy, double Vphi, double *eta,
        double *legRoll, double *legPitch, double *legYaw,
        double *footRoll, double *footPitch)
{
    if (tau <= 0.0)
    {
        *eta += (sin(tau)*(m_C[6] + m_C[7] * fmax(fabs(Vx),fabs(Vy))));
    } else {
        *eta += (sin(tau)*(m_C[8] + m_C[9] * fmax(fabs(Vx),fabs(Vy))));
    }
}

void MotionPattern::getLegSwing(int leg, double tau,
        double Vx, double Vy, double Vphi, double *eta,
        double *legRoll, double *legPitch, double *legYaw,
        double *footRoll, double *footPitch)
{
    double gamma=0.0;
    if (tau >= m_C[TAU0] && tau < m_C[TAU1])
    {
        gamma = cos(M_PI * (tau - m_C[TAU0]) / (m_C[TAU1] - m_C[TAU0]));
    } else if (tau >= m_C[TAU1] && tau < M_PI)
    {
        gamma = 2 * (tau - m_C[TAU1]) / (2 * M_PI - m_C[TAU1] + m_C[TAU0]) - 1.0;
    } else if (tau >= -M_PI && tau < m_C[TAU0])
    {
        gamma = 2 * (tau + 2 * M_PI - m_C[TAU1]) / (2 * M_PI - m_C[TAU1] + m_C[TAU0]) - 1.0;
    }

    if (Vx >= 0.0)
    {
        *legPitch += (gamma*Vx*m_C[10]);
    } else {
        *legPitch += (gamma*Vx*m_C[11]);
    }

    *legRoll += ((-gamma)*Vy*m_C[12] - (double)leg * fmax(fabs(Vy)*m_C[13], fabs(Vphi)*m_C[14]));

    *legYaw += (gamma*Vphi*m_C[15] - (double)leg * fabs(Vphi)*m_C[16]);
}

void MotionPattern::getLateralHipSwing(int leg, double tau,
        double Vx, double Vy, double Vphi, double *eta,
        double *legRoll, double *legPitch, double *legYaw,
        double *footRoll, double *footPitch)
{
    double tau_l, tau_r;
	
    if (tau < m_C[TAU0])
    {
        tau_l = tau - m_C[TAU1] + 2 * M_PI;
    } else if (tau > m_C[TAU1])
    {
        tau_l = tau - m_C[TAU1];
    } else {
        tau_l = 0.0;
    }

    if ((tau + M_PI) < m_C[TAU0])
    {
        tau_r = tau - m_C[TAU1] + 3 * M_PI;
    } else if ((tau + M_PI) > m_C[TAU1])
    {
        tau_r = tau - m_C[TAU1] + M_PI;
    } else {
        tau_r = 0.0;
    }

    double delta = m_C[TAU0] - m_C[TAU1] + 2 * M_PI;
    *legRoll += (m_C[17] * (sin(tau_l*M_PI/delta) - sin(tau_r*M_PI/delta)));
}


void MotionPattern::getLeaning(int leg, double tau,
        double Vx, double Vy, double Vphi, double *eta,
        double *legRoll, double *legPitch, double *legYaw,
        double *footRoll, double *footPitch)
{
    if (Vx >= 0.0)
    {
        *legPitch += (Vx*m_C[18]);
    } else {
        *legPitch += (Vx*m_C[19]);
    }
    *legRoll += (-Vphi*fabs(Vx)*m_C[20]);
}