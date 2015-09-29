/*
 * main.cpp
 *
 *  Created on: 2015-05-22
 *      Author: Rafael
 */

#include <cstdio>
#include "LinuxDARwIn.h"
#include "TauraWalking.h"
#include "AttitudeEstimation.h"
#include "util.h"

#define INI_FILE_PATH       "../../Data/config.ini"

#define U2D_DEV_NAME0 "/dev/ttyUSB0"
#define U2D_DEV_NAME1 "/dev/ttyUSB1"

using namespace Robot;

LinuxCM730 linux_cm730(U2D_DEV_NAME0);
CM730 cm730(&linux_cm730);

int main(void)
{
    minIni* ini = new minIni(INI_FILE_PATH);

    //////////////////// Framework Initialize ////////////////////////////
    if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    {
        linux_cm730.SetPortName(U2D_DEV_NAME1);
        if(MotionManager::GetInstance()->Initialize(&cm730) == false)
        {
            printf("Fail to initialize Motion Manager!\n");
            return 0;
        }
    }
	
	do{
        AttitudeEstimation::GetInstance()->SetController(&cm730);
    }while(&cm730==NULL);
	MotionManager::GetInstance()->AddModule((MotionModule*)AttitudeEstimation::GetInstance());
    //MotionManager::GetInstance()->AddModule((MotionModule*)TauraWalking::GetInstance());
	
    LinuxMotionTimer *motion_timer = new LinuxMotionTimer(MotionManager::GetInstance());
    motion_timer->Start();
    /////////////////////////////////////////////////////////////////////
    
    MotionManager::GetInstance()->LoadINISettings(ini);

	double step = 0.01;

    MotionManager::GetInstance()->SetEnable(true);
    
    bool parado=true;
    double Vx =0.0, Vy =0.0, Vphi = 0.0;

	TauraWalking::GetInstance()->Set_V_Target(Vx, Vy, Vphi);	
	
    while(true)
	{ 
		char c;
		
		c=tolower(util_getch());
		if(c=='q') break;
		
        if(c==' '){ 
            if(parado) TauraWalking::GetInstance()->Start(8);
            else TauraWalking::GetInstance()->Stop(8);
            parado=(!parado);
        }

		if(c=='2') {
			Vx+=step;
			printf("Vx+ \n");
		}
		else if(c=='1') {
			Vx-=step;
			printf("Vx- \n");
		}
		else if(c=='5') {
			Vy+=step;
			printf("Vy+ \n");
		}
		else if(c=='4') {
			Vy-=step;
			printf("Vy- \n");
		}
		else if(c=='8') {
			Vphi+=50*step;
			printf("Vphi+ \n");
		}
		else if(c=='7') {
			Vphi-=50*step;
			printf("Vphi- \n");
		}

        int aux = AttitudeEstimation::GetInstance()->Get_Fallen(); 
        if(aux) printf("Men down!!! (Side:%i)\n",aux);
        //printf("                                                                       \r");
		//printf("V:\tx = %.2f\t y = %.2f\t phi = %.2f\r", TauraWalking::GetInstance()->Get_Vx(), TauraWalking::GetInstance()->Get_Vy(), TauraWalking::GetInstance()->Get_Vphi());
		TauraWalking::GetInstance()->Set_V_Target(Vx, Vy, Vphi);	
		
	}
		
    printf("                                                                       \r");
    return 0;
}

