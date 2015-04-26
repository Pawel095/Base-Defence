#pragma once
#include <SDL.h>
#include <iostream>
#include <string>
#include "tools.h"

class Surowiec
{
    public:
        Surowiec();
        void render();
        bool isVisible=false;
        SDL_Renderer *ren;
        SDL_Texture *tex;
        SDL_Rect dim;
    protected:
    private:
};
