#pragma once
#include <SDL.h>
#include <iostream>
#include <string>
#include "tools.h"

class Baza
{
private:
	SDL_Rect B_HPBarClip[10];
public:
	bool B_isVisible;
	bool B_isFriend = true;
	string B_texturePath;
	string B_relativeTexturePath = "\\RES\\baza.png";
	string B_HPBarPath;
	string B_relativeHPBarPath = "\\RES\\HP bar.png";
	SDL_Texture *B_texture;
	SDL_Texture *B_HPBarTexture;
	SDL_Texture *B_font;
	SDL_Renderer *B_renderer;
	int B_x, B_y;
	int B_HP;
	Baza();
	Baza(string workingdir, SDL_Renderer *ren, int x, int y);
	void B_render();
	void init();
};

