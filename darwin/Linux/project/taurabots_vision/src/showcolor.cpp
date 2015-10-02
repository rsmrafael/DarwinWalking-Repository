#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "BlobScan.h"
#include "Image.h"
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

    if (argc != 4)
    {
        printf("Usage %s input.jpg sample.color output.jpg\n", argv[0]);
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

    BlobScan blob(rgb_image->m_ImageData, width, height);
    printf("Reading color from %s\n", argv[2]);
    blob.ColorFromFile(argv[2]);

    printf("Scanning image %s\n", argv[1]);
    blob.Scan();

    printf("Drawing results\n");
    blob.HighlightAllBlobs(rgb_image->m_ImageData);

    printf("Writing file %s\n", argv[3]);
    write_jpeg_file(argv[3], rgb_image->m_ImageData, width, height, Image::RGB_PIXEL_SIZE);
    
    return 0;
}

