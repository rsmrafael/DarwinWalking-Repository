#include "SDLWindow.h"
#include <cstdio>

SDLWindow *SDLWindow::mSingleInstance = new SDLWindow();

SDLWindow::SDLWindow()
{
    SDL_Init( SDL_INIT_EVERYTHING );
}

unsigned char SDLWindow::HandleEvents()
{
    SDL_Event event;
    int value = 0;
    while (SDL_PollEvent(&event))
    {
        switch (event.type){
            case SDL_KEYUP:
                value = (int)event.key.keysym.sym;
                break;
            default:
                break;
        }
    }
    return value;
}

int SDLWindow::SetVideoMode( int width, int height, int bytes_per_pixel )
{
    mWidth = width;
    mHeight = height;
    if ((mScreen = SDL_SetVideoMode( width, height, 8*bytes_per_pixel, SDL_SWSURFACE )) == NULL)
    {
        return -1;
    }
    return 0;
}

void SDLWindow::Delay_ms(int time)
{
    SDL_Delay(time);
}

void SDLWindow::Update()
{
    SDL_Flip( mScreen );
}

void SDLWindow::CopyRGB24( void *pixels, size_t size )
{
    SDL_LockSurface( mScreen );
    for (size_t i = 0 ; i < size / 3 ; i++)
    {
	char *orig = (char *)pixels + 3*i;
	// Flips the image upside down (robot's camera position)
	int row = i / (mWidth);
	int col = i % (mWidth);
        char *dest = (char *)mScreen->pixels + 3*(row*mWidth + col);
	dest[0] = orig[2];
	dest[1] = orig[1];
	dest[2] = orig[0];
    }
    SDL_UnlockSurface( mScreen );
}

SDLWindow::~SDLWindow()
{
    SDL_Quit();
}

