#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "FeaturePointFinder.h"
#include "jpeg_util.h"

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

    if (argc != 3)
    {
        printf("Usage %s input.jpg output.jpg\n", argv[0]);
        return -1;
    }

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

    FeaturePointFinder fpfinder;
    printf("Processing\n");
    fpfinder.ProcessFrame(rgb_image);
    printf("Drawing results\n");
    fpfinder.DrawFeaturePoints(rgb_image);

    printf("Writing file %s\n", argv[2]);
    write_jpeg_file(argv[2], rgb_image->m_ImageData, width, height, Image::RGB_PIXEL_SIZE);
    
    return 0;
}

