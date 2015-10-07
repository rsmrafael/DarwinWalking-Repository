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
#include <time.h>

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

	AttitudeEstimation::GetInstance()->Initialize();


    while(true)
	{

		char c;
		//time = now
		AttitudeEstimation::GetInstance()->Process();
		//time = now - time
		//sleep(8-time milissegundos);

		c=tolower(util_getch());
		if(c=='q') break;

	}

    printf("                                                                       \r");
    return 0;
}
