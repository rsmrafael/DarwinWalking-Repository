/*
 * main.cpp
 *
 *  Created on: 2011. 1. 4.
 *      Author: robotis
 */

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <libgen.h>
#include <signal.h>

#include "mjpg_streamer.h"
#include "LinuxDARwIn.h"
#include "TauraTracker.h"

#ifdef MX28_1024
#define MOTION_FILE_PATH    "../../../Data/motion_1024.bin"
#else
#define MOTION_FILE_PATH    "../../../Data/motion_4096.bin"
#endif

#define INI_FILE_PATH       "../../../Data/config.ini"
#define SCRIPT_FILE_PATH    "script.asc"

#define U2D_DEV_NAME0       "/dev/ttyUSB0"
#define U2D_DEV_NAME1       "/dev/ttyUSB1"

LinuxCM730 linux_cm730(U2D_DEV_NAME0);
CM730 cm730(&linux_cm730);

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
    {
        if(chdir(dirname(exepath)))
            fprintf(stderr, "chdir error!! \n");
    }
}

void sighandler(int sig)
{
    exit(0);
}

int main(void)
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    change_current_dir();

    minIni* ini = new minIni(INI_FILE_PATH);
    Image* rgb_output = new Image(Camera::WIDTH, Camera::HEIGHT, Image::RGB_PIXEL_SIZE);

    LinuxCamera::GetInstance()->Initialize(0);
    LinuxCamera::GetInstance()->SetCameraSettings(CameraSettings());    // set default
    LinuxCamera::GetInstance()->LoadINISettings(ini);                   // load from ini

    mjpg_streamer* streamer = new mjpg_streamer(Camera::WIDTH, Camera::HEIGHT);

    ColorFinder* ball_finder = new ColorFinder();
    ball_finder->LoadINISettings(ini);
    httpd::ball_finder = ball_finder;

    TauraTracker tracker = TauraTracker();
    BallFollower follower = BallFollower();

    httpd::ini = ini;

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

    Walking::GetInstance()->LoadINISettings(ini);

    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
    MotionManager::GetInstance()->AddModule((MotionModule*)Head::GetInstance());
    MotionManager::GetInstance()->AddModule((MotionModule*)Walking::GetInstance());

    LinuxMotionTimer *motion_timer = new LinuxMotionTimer(MotionManager::GetInstance());
    motion_timer->Start();
    /////////////////////////////////////////////////////////////////////
    
    MotionManager::GetInstance()->LoadINISettings(ini);

    Action::GetInstance()->LoadFile((char*)MOTION_FILE_PATH);

    Action::GetInstance()->m_Joint.SetEnableBody(true, true);
    MotionManager::GetInstance()->SetEnable(true);

    cm730.WriteByte(CM730::P_LED_PANNEL, 0x01|0x02|0x04, NULL);

    LinuxActionScript::PlayMP3("../../../Data/mp3/Autonomous soccer mode.mp3");
    Action::GetInstance()->Start(15);
    while(Action::GetInstance()->IsRunning()) usleep(8*1000);

    while(1)
    {
        Point2D ball_pos;

        LinuxCamera::GetInstance()->CaptureFrame();
        memcpy(rgb_output->m_ImageData, LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageData, LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageSize);

        tracker.Process(ball_finder->GetPosition(LinuxCamera::GetInstance()->fbuffer->m_HSVFrame));

	for(int i = 0; i < rgb_output->m_NumberOfPixels; i++)
	{
		if(ball_finder->m_result->m_ImageData[i] == 1)
		{
			rgb_output->m_ImageData[i*rgb_output->m_PixelSize + 0] = 255;
			rgb_output->m_ImageData[i*rgb_output->m_PixelSize + 1] = 128;
			rgb_output->m_ImageData[i*rgb_output->m_PixelSize + 2] = 0;
		}
	}

        streamer->send_image(rgb_output);

	if(Action::GetInstance()->IsRunning() == 0)
	{
		Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
		Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);

		// This makes the robot walk toweard ball_position
		//follower.Process(tracker.ball_position);

		fprintf(stderr, "Ball position: %+3.3f,%+3.3f\n",
			tracker.ball_position.X, tracker.ball_position.Y);
		fprintf(stderr, "Pan angle: %+3.3f\n", Head::GetInstance()->GetPanAngle());

		if(follower.KickBall != 0)
		{
			Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
			Action::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);

			if(follower.KickBall == -1)
			{
				Action::GetInstance()->Start(12);   // RIGHT KICK
				fprintf(stderr, "RightKick! \n");
			}
			else if(follower.KickBall == 1)
			{
				Action::GetInstance()->Start(13);   // LEFT KICK
				fprintf(stderr, "LeftKick! \n");
			}
		}
	}
    }

    return 0;
}
