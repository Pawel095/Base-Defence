#include "Baza.h"

Baza::Baza(string workingdir, SDL_Renderer *ren, int x, int y){
	B_isVisible = false;
	B_texturePath = workingdir + B_relativeTexturePath;
	B_HPBarPath = workingdir + B_relativeHPBarPath;
	B_renderer = ren;
	B_x = x;
	B_y = y;
	init();
}

void Baza::init(){
	B_texture = loadTexture(B_texturePath, B_renderer);
	B_HPBarTexture = loadTexture(B_HPBarPath, B_renderer);

	//HPBarClip
	int iterator = 0;
	for (int y = 0; y < 32; y += 16){
		for (int x = 0; x < 250; x += 50){
			B_HPBarClip[iterator].x = x;
			B_HPBarClip[iterator].y = y;
			B_HPBarClip[iterator].w = 50;
			B_HPBarClip[iterator].h = 16;
			iterator++;
		}
	}
}

void Baza::B_render(){
	if (B_isFriend == true && B_isVisible == true) {
		SDL_Rect clip;
		clip.x = 0; clip.y = 0; clip.w = 50; clip.h = 50;


		renderTexture(B_texture, B_renderer, B_x, B_y, &clip);
		renderTexture(B_HPBarTexture, B_renderer, B_x, B_y - 16, &B_HPBarClip[B_HP/10]);
		renderWord(B_HP, B_x, B_y-11, B_font, B_renderer);
	}
}
