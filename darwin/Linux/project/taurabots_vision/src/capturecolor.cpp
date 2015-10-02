#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "LinuxDARwIn.h"
#include "BlobScan.h"
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

    if (argc != 3 && argc != 2)
    {
        printf("Usage\n");
        printf("  - From file:   %s input.jpg output.color\n", argv[0]);
        printf("  - From camera: %s output.color\n", argv[0]);
        return -1;
    } else if (argc == 3)
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

        BlobScan blob(rgb_image->m_ImageData, width, height);
        printf("Reading colors from the image\n");
        blob.GetColorFromRectangle(5,5,width-5,height-5);
        for (int i = 0 ; i < 5 ; i++)
            blob.InflateColor();

        printf("Writing file %s\n", argv[2]);
        blob.ColorToFile(argv[2]);
    } else {
        minIni *ini = new minIni(INI_FILE_PATH);

        LinuxCamera::GetInstance()->Initialize(0);
        LinuxCamera::GetInstance()->SetCameraSettings(CameraSettings());
        LinuxCamera::GetInstance()->LoadINISettings(ini);

        Image *rgb_image = new Image(Camera::WIDTH, Camera::HEIGHT,
                                     Image::RGB_PIXEL_SIZE);

        bool has_color = false;
        BlobScan blob(rgb_image->m_ImageData, Camera::WIDTH, Camera::HEIGHT);
        if (blob.ColorFromFile(argv[1]) == 0)
        {
            printf("Found existing color in %s.\n", argv[1]);
            has_color = true;
        }

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
                case ' ':
                    printf("Capturing color now\n");
                    blob.GetColorFromRectangle(
                        5, 5, Camera::WIDTH-5, Camera::HEIGHT-5);
                    has_color = true;
                    break;
                case '+':
                case '=':
                case '>':
                case '.':
                    if (has_color)
                    {
                    printf("Inflating color now\n");
                        blob.InflateColor();
                    }
                    break;
                case 127:
                    if (has_color)
                    {
                        printf("Reseting color now\n");
                        blob.ResetColor();
                        has_color = false;
                    }
                    break;
                case 27:
                    quit = true;
                    break;
                case '\r':
                    quit = true;
                    break;
                default:
                    break;
            }

            if (has_color)
            {
                blob.Scan();
                blob.HighlightAllBlobs(rgb_image->m_ImageData);
            }

            SDLWindow::GetInstance()->CopyRGB24(
                   rgb_image->m_ImageData,
                   rgb_image->m_ImageSize);

            SDLWindow::GetInstance()->Update();

            SDLWindow::GetInstance()->Delay_ms(20);
        }

        if (key == '\r')
        {
            printf("Writing file %s\n", argv[1]);
            blob.ColorToFile(argv[1]);
        } else {
            printf("Cancel, nothing saved.\n");
        }

        return 0;
    }
    
    return 0;
}

