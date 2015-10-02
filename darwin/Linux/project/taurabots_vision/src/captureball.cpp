#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "LinuxDARwIn.h"
#include "BallFinder.h"
#include "jpeg_util.h"
#include "SDLWindow.h"

#define INI_FILE_PATH "./config.ini"

using namespace Robot;

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

    if (argc != 4 && argc != 2)
    {
        printf("Usage\n");
        printf("  - From file:   %s input.jpg input.color output.jpg\n", argv[0]);
        printf("  - From camera: %s input.color\n", argv[0]);
        return -1;
    } else if (argc == 4)
    {
        printf("Reading file %s\n", argv[1]);
        int width, height;
        unsigned char *rgb_buffer_from_file =
            read_jpeg_file( argv[1], &width, &height );
        printf("Size %d x %d\n", width, height);

        printf("Creating Image class\n");
        Image *rgb_image = new Image(width, height, Image::RGB_PIXEL_SIZE);
        printf("Copying the data\n");
        memcpy(rgb_image->m_ImageData, rgb_buffer_from_file,
               width*height*Image::RGB_PIXEL_SIZE);

        printf("Creating BlobScan class\n");
        BlobScan blob(rgb_image->m_ImageData, width, height);
        if (blob.ColorFromFile(argv[2]) == 0)
        {
            printf("Found existing color in %s.\n", argv[2]);
        } else {
            printf("Problem reading existing color in %s.\n", argv[2]);
            return -1;
        }

        printf("Creating the FieldLimitsFinder class\n");
        FieldLimitsFinder flims(rgb_image);
        flims.SetFieldColorMap(blob.GetColorMap());

        printf("Finding the limits of the field\n");
        flims.FindLimits(rgb_image);

        printf("Creating the BallFinder class\n");
        BallFinder bfinder(rgb_image, &flims);

        printf("Finding the candidate balls\n");
        bfinder.FindCandidates();

        printf("Drawing the candidate balls\n");
        bfinder.DrawCandidates(rgb_image);

        printf("Drawing limits of the field\n");
        flims.DrawLimits(rgb_image);

        printf("Writing file %s\n", argv[3]);
        write_jpeg_file(argv[3], rgb_image->m_ImageData,
                        width, height, Image::RGB_PIXEL_SIZE);
        
    } else {
        minIni *ini = new minIni(INI_FILE_PATH);

        LinuxCamera::GetInstance()->Initialize(0);
        LinuxCamera::GetInstance()->SetCameraSettings(CameraSettings());
        LinuxCamera::GetInstance()->LoadINISettings(ini);

        Image *rgb_image = new Image(Camera::WIDTH, Camera::HEIGHT,
                                     Image::RGB_PIXEL_SIZE);

        BlobScan blob(rgb_image->m_ImageData, Camera::WIDTH, Camera::HEIGHT);
        if (blob.ColorFromFile(argv[1]) == 0)
        {
            printf("Found existing color in %s.\n", argv[1]);
        } else {
            printf("Problems reading existing color in %s.\n", argv[1]);
            return -1;
        }

        printf("Creating the FieldLimitsFinder class\n");
        FieldLimitsFinder flims(rgb_image);
        flims.SetFieldColorMap(blob.GetColorMap());

        printf("Creating the BallFinder class\n");
        BallFinder bfinder(rgb_image, &flims);

        printf("Creating the SDL window\n");
        SDLWindow::GetInstance()->SetVideoMode( Camera::WIDTH, Camera::HEIGHT,
		                                        Image::RGB_PIXEL_SIZE );
        bool quit = false;
        unsigned char key = 0;
        while (!quit)
        {

            LinuxCamera::GetInstance()->CaptureFrame();

            memcpy(rgb_image->m_ImageData,
                   LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageData,
                   rgb_image->m_ImageSize);

            key = SDLWindow::GetInstance()->HandleEvents();
            switch (key)
            {
                case 27:
                    quit = true;
                    break;
                case '\r':
                    quit = true;
                    break;
                default:
                    break;
            }

            flims.FindLimits(rgb_image);
            bfinder.FindCandidates();

            flims.DrawLimits(rgb_image);
            bfinder.DrawCandidates(rgb_image);

            SDLWindow::GetInstance()->CopyRGB24(
                   rgb_image->m_ImageData,
                   rgb_image->m_ImageSize);

            SDLWindow::GetInstance()->Update();

            SDLWindow::GetInstance()->Delay_ms(20);
        }
    }
    
    return 0;
}

