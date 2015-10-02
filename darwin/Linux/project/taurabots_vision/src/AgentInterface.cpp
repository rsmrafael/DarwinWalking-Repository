#include "AgentInterface.h"
#include <signal.h>

#include "mjpg_streamer.h"
#include "LinuxDARwIn.h"

#include "Omniwalk.h"
#include "MotionPattern.h"

#include "BlobScan.h"
#include "BallFinder.h"
#include "FieldLimitsFinder.h"
#include "LinesFinder.h"

#define MOTION_FILE_PATH    "../../../Data/motion_4096.bin"
#define FIELD_COLOR_FILE    "./fieldcolor.blob" //TODO TauraBots

#define DEVNAME0 "/dev/ttyUSB0"
#define DEVNAME1 "/dev/ttyUSB1"

LinuxCM730 linux_cm730(DEVNAME0);
CM730 cm730(&linux_cm730);

void sighandler(int sig)
{
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    Image* rgb_output = 
        new Image(Camera::WIDTH, Camera::HEIGHT, Image::RGB_PIXEL_SIZE);

    minIni* ini = new minIni(INI_FILE_PATH);

    LinuxCamera::GetInstance()->Initialize(0);
    LinuxCamera::GetInstance()->SetCameraSettings(CameraSettings());
    LinuxCamera::GetInstance()->LoadINISettings(ini);

    mjpg_streamer* streamer =
        new mjpg_streamer(Camera::WIDTH, Camera::HEIGHT);

    //TauraBallTracker tracker(); TODO TauraBots
    //TauraBallFollower follower(); TODO TauraBots

    httpd::ini = ini;

    if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    {
        linux_cm730.SetPortName(DEVNAME1);
        if(MotionManager::GetInstance()->Initialize(&cm730) == false)
        {
            printf("Fail to initialize Motion Manager!\n");
            return 0;
        }
    }
    
    do {
        Omniwalk::GetInstance()->SetController(&cm730);
    } while (&cm730 == NULL);
    MotionManager::GetInstance()->AddModule((MotionModule*)Omniwalk::GetInstance());
    //Omniwalk::GetInstance()->LoadINISettings(ini); //TODO TauraBots

    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
    MotionManager::GetInstance()->AddModule((MotionModule*)Head::GetInstance());

    LinuxMotionTimer *motion_timer =
        new LinuxMotionTimer(MotionManager::GetInstance());
    motion_timer->Start();

    Action::GetInstance()->LoadFile((char*)MOTION_FILE_PATH);

    Action::GetInstance()->m_Joint.SetEnableBody(true, true);
    MotionManager::GetInstance()->SetEnable(true);

    cm730.WriteByte(CM730::P_LED_PANNEL, 0x01|0x02|0x04, NULL);

    LinuxActionScript::PlayMP3("../../../Data/mp3/Demonstration ready mode.mp3");
    Action::GetInstance()->Start(15);
    while(Action::GetInstance()->IsRunning()) usleep(8*1000);

    //AgentInterface ai;

    BlobScan blob(rgb_image->m_ImageData, width, height);
    if (blob.ColorFromFile(FIELD_COLOR_FILE) == 0)
    {
        printf("Found existing field color file");
    } else {
        printf("Problem reading existing field color file");
        return -1;
    }

    FieldLimitsFinder flims(rgb_image);
    flims.SetFieldColorMap(blob.GetColorMap());

    BallFinder bfinder(rgb_image, &flims);

    LinesFinder flines(&flims);

    while (true)
    {
        StatusCheck::Check(cm730);

        LinuxCamera::GetInstance()->CaptureFrame();
        memcpy(rgb_output->m_ImageData, LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageData, LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageSize);

        // Find limits of the field
        flims.FindLimits(rgb_image);
        // Find the ball candidates
        bfinder.FindCandidates();
        // Find the lines
        flines.FindLines(rgb_image);
        // Find pole candidates
        flines.FindPoleCandidates();

        //ai.SendToAgent(bfinder->GetBallCandidates(), flines->GetPoleCandidates());
        // TODO TauraBots
        //
        // ai.GetCommands(); ???? TODO TauraBots;

        //tracker.Process(ball_pos); //TODO TauraBots

        // Omniwalk::GetInstance()->Set_V_Target(Vx, Vy, Vphi); //TODO TauraBots
        // if (Vx == 0.0 && Vy == 0.0 && Vphi == 0.0
        //     && !(Omniwalk::GetInstance()->Stopped()
        //          || Omniwalk::GetInstance()->Stopping()))
        // {
        //      Omniwalk::GetInstance()->Stop(8);
        // } else if ((Vx != 0.0 || Vy != 0.0 || Vphi != 0.0)
        //            && !(Omniwalk::GetInstance()->Started()
        //                 || Omniwalk::GetInstance()->Starting())
        // {
        //      Omniwalk::GetInstance()->Start(8);
        // }

        //bfinder.DrawCandidates(rgb_image);
        //flines.DrawPoleCandidates(rgb_image);
        //flims.DrawLimits(rgb_image);
    }

    return 0;
}

