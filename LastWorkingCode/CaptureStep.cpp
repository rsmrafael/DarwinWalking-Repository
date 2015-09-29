
#include <cmath>
#include <cstdio>

#include "Kinematics.h"
#include "ars.h"
#include "CaptureStep.h"
#include "Omniwalk.h"

#define G 9.81  // m/s^2
#define L (Robot::Kinematics::LEG_LENGTH/1000.0) // m
#define C28 20.0 // Distance, in milimeters, between the pelvis and the center of mass in the Z axis
#define QAngle 0.001
#define QGyro 0.003
#define RAngle 69
#define AccThreshold 0.2            // Consider acc if Norm(ay||ax,az)>AccThreshold*norm(ax,ay,az)
#define PitchOffset -0.3               // aproximadamente -0.3 nos testes iniciais
#define ComplementaryFilter 0.05    // 0 - only gyro | 1 - only Acc

#define SQR(x) ((x)*(x))


CaptureStep *CaptureStep::m_UniqueInstance = new CaptureStep();

CaptureStep::CaptureStep(){
	W0 = sqrt(G/L);
	CoM[0] = 0;
	CoM[1] = 0;
	dCoMdt[0] = 0;
	dCoMdt[1] = 0;
	Vx=0;
	Vy=0;
	dt=8.0/1000.0;
    T = dt*M_PI/C25; //C25 comes from Omniwalk.h
}

CaptureStep::~CaptureStep(){ 
}

double CaptureStep::calculateCOM(int axis, double Roll, double Pitch, double X, double Y, double Z){
	
	if(axis==0){ //Calcular eixo x
		return (cos(Pitch)*X+sin(Roll)*sin(Pitch)*Y+sin(Pitch)*cos(Roll)*Z);
	}
	
	if(axis==1){ //calcular eixo y
		return (cos(Roll)*Y-sin(Roll)*Z);
	}
	
	return 0;
}

void CaptureStep::update(double LegX, double LegY, double LegZ, double eta, char SuportLeg){
	double NewValue;
	double Foot[2];
	int LegSign=0;

    
	if ((SuportLeg=='r')||(SuportLeg=='R')) LegSign=1;	
    else if ((SuportLeg=='l')||(SuportLeg=='L')) LegSign=-1;
	
	//W0 = sqrt(G/((1-eta)*L));
	W0 = sqrt(G/(L);
	// --------------- attitude estimation ----------------------------
	
	double Roll = AttitudeEstimation::GetInstance()->Get_Roll();
	double Pitch = AttitudeEstimation::GetInstance()->Get_Pitch();

	// ------------- Pelvis with reference in the support foot ------------
	
	double X = -LegX/1000.0;
	double Y = -(LegY+LegSign*(Robot::Kinematics::LEG_SIDE_OFFSET/2.0))/1000.0;
	double Z = -(LegZ+C28)/1000.0;
	
	// ------------------------------ X axis ------------------------------
	NewValue = calculateCOM(0, Roll, Pitch, X, Y, Z);
    dCoMdt[0] = NewValue - CoM[0];
	CoM[0] = NewValue;
    XCoM[0] = CoM[0] + dCoMdt[0]/W0;
	
	double Sc = Vx*T;
	double Bx = Sc / (exp(W0*T)-1);	
	
	Foot[0] = XCoM[0]-Bx; // Inertial plan (x axis)
	
	// ------------------------------ Y axis ------------------------------
	
	NewValue = calculateCOM(1, Roll, Pitch, X, Y, Z);
    dCoMdt[1] = NewValue - CoM[1];
	CoM[1] = NewValue;
	XCoM[1] = CoM[1] + dCoMdt[1]/W0;
	
	double Wc = Robot::Kinematics::LEG_SIDE_OFFSET/1000;
	Wc-= LegSign*Vy*T;
	double By = Wc / (exp(W0*T)+1);
	
	Foot[1] = XCoM[1]-LegSign*By; // verificar sinal
	
	// ------------------ Update Ux & Uy ------------------------------------
	
	Ux = 1000*(cos(Pitch)*Foot[0] + sin(Roll)*sin(Pitch)*Foot[1])-LegX;
	Uy = 1000*(cos(Roll)*Foot[1])+LegSign*Robot::Kinematics::LEG_SIDE_OFFSET-LegY;

	//printf("                                                                       \r");
    //printf("Roll:\t%.2f\tPitch:\t%.2f\n", Roll,Pitch );
    //printf("Leg: %c\tX: %.2f\tY: %.2f\tZ: %.2f\n",SuportLeg,X*1000.0,Y*1000.0,Z*1000);
    //printf("Foot: x= %.2f  , y= %.2f, z= %.2f\n",LegX,LegY,LegZ);
    //printf("Leg: %c\tCoM in X: %.2f\tCoM in Y: %.2f\n",SuportLeg,CoM[0]*1000.0,CoM[1]*1000.0);
    // printf("Leg: %c\tXCoM in X: %.2f\tXCoM in Y: %.2f\n",SuportLeg,XCoM[0]*1000.0,XCoM[1]*1000.0);
    //printf("Leg:%c\tUx: %.2f\tUy: %.2f\n",SuportLeg,Ux,Uy);
}
