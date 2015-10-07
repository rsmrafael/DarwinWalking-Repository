/*
 * main.cpp
 *
 *  Created on: 2015-05-22
 *      Author: Rafael
 *
 *	Edited on: 2015-10-07
 *		Nícolas
 */

#include <cstdio>
#include "LinuxDARwIn.h"
#include "TauraWalking.h"
#include "AttitudeEstimation.h"
#include "util.h"
//Library included to make him stand up from falling
#include "action.h"

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
	MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
	//Last added module commands motors
    MotionManager::GetInstance()->AddModule((MotionModule*)TauraWalking::GetInstance());

	
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
        int Fallen = AttitudeEstimation::GetInstance()->Get_Fallen(); 
        if(Fallen) { //Get_Fallen() returns -2,-1,1,2

			//m_is_started == 1 ??
			printf("Men down!!! (Side:%i)\n",aux);//if(MotionStatus::FALLEN != STANDUP && m_cur_mode == SOCCER && m_is_started == 1);
			
			TauraWalking::GetInstance()->Stop(8);//Stop TauraWalking
			
			while(TauraWalking::GetInstance()->IsRunning() == 1) {}//Wait until he actually stops TauraWalking
			
			Action::GetInstance()->m_Joint.SetEnableBody(true, true);//Enable all Body Control to Action module
			
		  //if (Fallen =  0){ /*Do nothing*/ } 				     //STAND UP
			if (Fallen = -1){Action::GetInstance()->Start(201);} //BACK FALL
			if (Fallen =  1){Action::GetInstance()->Start(200);} //FRONT FALL
			if (Fallen = -2){Action::GetInstance()->Start();} //LEFT FALL
			if (Fallen =  2){Action::GetInstance()->Start();} //RIGHT FALL
			
			while(Action::GetInstance()->IsRunning() == 1) {} //Giving time to end motion in 8000us = 8ms
			
			//Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);//Give back control of Head (MX28 number 19,20) to Head module
			
			TauraWalking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);//Give back control of Body (MX28 numbers 0 to 18) to TauraWalking module
		} 
				
        //printf("                                                                       \r");
		//printf("V:\tx = %.2f\t y = %.2f\t phi = %.2f\r", TauraWalking::GetInstance()->Get_Vx(), TauraWalking::GetInstance()->Get_Vy(), TauraWalking::GetInstance()->Get_Vphi());
		TauraWalking::GetInstance()->Set_V_Target(Vx, Vy, Vphi);	
		
	}
		
    printf("                                                                       \r");
    return 0;
}

