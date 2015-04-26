#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include "tools.h"

using namespace std;

class Enemy{
private:
	SDL_Rect E_HPBarClip[10];
public:
	//constructor################################

	Enemy();
	Enemy(string workingdir, SDL_Renderer *ren);
	Enemy(string workingdir, SDL_Renderer *ren, int x, int y);
	//constructor################################

	bool E_isVisible;
	bool E_colides;
	bool E_targetAcquired = false;
	bool *E_isPaused;
	string E_texturePath;
	string E_relativeTexturePath = "\\RES\\Enemy.png";
	string E_HPBarPath;
	string E_relativeHPBarPath = "\\RES\\HP bar.png";
	SDL_Texture *E_texture;
	SDL_Texture *E_HPBarTexture;
	SDL_Renderer *E_renderer;
	void init();
	int E_targetNo;
	int E_x, E_y;
	int E_HP;
	int E_moveToX = 0, E_moveToY = 0;
	int E_range;
	int timer = 80;
	int *E_pointsPointer;
	void E_render();
protected:

};
