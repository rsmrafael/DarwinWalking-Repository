#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "SDL/SDL.h"

class SDLWindow {
    private:
        SDLWindow();
        ~SDLWindow();
        static SDLWindow *mSingleInstance;
        SDL_Surface *mScreen;
        int mWidth, mHeight;
    public:
        static SDLWindow *GetInstance() { return mSingleInstance; }
        int SetVideoMode( int width, int height, int bytes_per_pixel );
        void CopyRGB24( void *pixels, size_t size );
        void Update();
        unsigned char HandleEvents();
        void Delay_ms(int time);
};

#endif

