#include <signal.h>

#include "mjpg_streamer.h"
#include "LinuxDARwIn.h"
#include "FeaturePointFinder.h"

#define INI_FILE_PATH "./config.ini"

void sighandler(int sig)
{
    exit(0);
}

using namespace Robot;

int main(int argc, char *argv[])
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    minIni *ini = new minIni(INI_FILE_PATH);

    Image *rgb_output = new Image(Camera::WIDTH,
                                  Camera::HEIGHT,
                                  Image::RGB_PIXEL_SIZE);

    LinuxCamera::GetInstance()->Initialize(0);
    LinuxCamera::GetInstance()->SetCameraSettings(CameraSettings());
    LinuxCamera::GetInstance()->LoadINISettings(ini);

    mjpg_streamer *streamer = new mjpg_streamer(Camera::WIDTH,
                                                Camera::HEIGHT);

    httpd::ini = ini;

    FeaturePointFinder fpfinder;

    while (1)
    {
        LinuxCamera::GetInstance()->CaptureFrame();

        memcpy(rgb_output->m_ImageData, 
           LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageData,
           LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageSize);

        fpfinder.ProcessFrame(rgb_output);
        fpfinder.DrawFeaturePoints(rgb_output);

        streamer->send_image(rgb_output);
    }

    return 0;
}

