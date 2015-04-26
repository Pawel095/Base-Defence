#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include "Pocisk.h"
#include "tools.h"

using namespace std;

class Friend{
private:
	SDL_Rect F_HPBarClip[10];
	SDL_Rect F_RangaClip[6];
public:
	//constructor################################

	Friend();
	Friend(string workingdir, SDL_Renderer *ren);
	Friend(string workingdir, SDL_Renderer *ren, int x, int y);
	//constructor################################

	bool F_isSelected;
	bool F_isVisible;
	bool F_colides;
	bool *F_isPaused;
	string F_texturePath;
	string F_relativeTexturePath = "\\RES\\friend.png";
	string F_HPBarPath;
	string F_relativeHPBarPath = "\\RES\\HP bar.png";
	SDL_Texture *F_rangi;
	SDL_Texture *F_texture;
	SDL_Texture *F_HPBarTexture;
	SDL_Renderer *F_renderer;
	void init();
	void F_render();
	int ranga=0;
	int *awansCount;
	int kills=0;
	int F_x, F_y;
	int F_attackTimer = 0;
	int range = 300;
	int F_HP;
	int F_moveToX = 0, F_moveToY = 0;
	Target F_cel;

protected:

};
