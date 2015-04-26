#pragma once
#include <SDL.h>
#include <iostream>
#include <string>
#include "tools.h"

class Pocisk
{
    public:
        Pocisk();
        void render();
        bool *isPaused;
        bool isVisible = false;
        double P_x;
        double P_y;
        int P_moveToX;
        int P_moveToY;
        double P_angle;
        SDL_Texture *P_tex;
        SDL_Renderer *P_ren;
        void licz();
    private:
        double dx=1;
        double dy=1;
};
