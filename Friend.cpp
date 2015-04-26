#include "Friend.h"


void Friend::F_render(){
    if(kills>=20 && ranga <5){
        kills=0;
        ranga++;
        *awansCount+=1;
    }
	if (F_isSelected == false && F_isVisible == true ) {
		SDL_Rect clip;
		clip.x = 0; clip.y = 0; clip.w = 50; clip.h = 50;

		if (*F_isPaused == true){
			renderTexture(F_texture, F_renderer, F_x, F_y, &clip);
			renderTexture(F_HPBarTexture, F_renderer, F_x, F_y - 16, &F_HPBarClip[F_HP]);
			return void();
		}

		//przesuwanie
		if (F_x < F_moveToX)
			F_x++;
		else if (F_x > F_moveToX)
			F_x--;
		if (F_y < F_moveToY)
			F_y++;
		else if (F_y>F_moveToY)
			F_y--;

		renderTexture(F_texture, F_renderer, F_x, F_y, &clip);
		//render HP Bar;
        renderTexture(F_HPBarTexture, F_renderer, F_x, F_y - 16, &F_HPBarClip[F_HP]);
        renderTexture(F_rangi,F_renderer,(F_x+50)-32,F_y,&F_RangaClip[ranga]);
	}if (F_isSelected == true && F_isVisible == true) {
		SDL_Rect clip;
		clip.x = 50; clip.y = 0; clip.w = 50; clip.h = 50;

		if (*F_isPaused == true){
			renderTexture(F_texture, F_renderer, F_x, F_y, &clip);
			renderTexture(F_HPBarTexture, F_renderer, F_x, F_y - 16, &F_HPBarClip[F_HP]);
			return void();
		}

		//przesuwanie
		if (F_x < F_moveToX)
			F_x++;
		else if (F_x > F_moveToX)
			F_x--;
		if (F_y < F_moveToY)
			F_y++;
		else if (F_y>F_moveToY)
			F_y--;


		renderTexture(F_texture, F_renderer, F_x, F_y, &clip);
		//render HP Bar;
        renderTexture(F_HPBarTexture, F_renderer, F_x, F_y - 16, &F_HPBarClip[F_HP]);
        renderTexture(F_rangi,F_renderer,(F_x+50)-32,F_y,&F_RangaClip[ranga]);
	}
}


void Friend::init(){
	F_texture=loadTexture(F_texturePath,F_renderer);
	F_HPBarTexture = loadTexture(F_HPBarPath,F_renderer);
	//HPBarClip
	int iterator = 0;
	for (int y = 0; y < 32; y += 16){
		for (int x = 0; x < 250; x += 50){
			F_HPBarClip[iterator].x = x;
			F_HPBarClip[iterator].y = y;
			F_HPBarClip[iterator].w = 50;
			F_HPBarClip[iterator].h = 16;
			iterator++;
		}
	}
	//rangaClip
	iterator=0;
	for(int x=0;x<192;x+=32){
        F_RangaClip[iterator].x=x;
        F_RangaClip[iterator].y=0;
        F_RangaClip[iterator].w=32;
        F_RangaClip[iterator].h=16;
        iterator++;
	}
}
Friend::Friend(){
	F_isSelected = false;
	F_texturePath = ' ';
	F_renderer = NULL;
	F_x = 0;
	F_y = 0;
	F_HP = 10;
	F_isVisible = false;
}


Friend::Friend(string workingdir, SDL_Renderer *ren){
	F_isSelected = false;
	F_isVisible = false;
	F_texturePath = workingdir + F_relativeTexturePath;
	F_HPBarPath = workingdir + F_relativeHPBarPath;
	F_renderer = ren;
	F_x = 0;
	F_y = 0;
	init();
}
Friend::Friend(string workingdir, SDL_Renderer *ren, int x, int y) {
	F_isSelected = false;
	F_isVisible = false;
	F_texturePath = workingdir + "\\RES\\friend.png";
	F_HPBarPath = workingdir + F_relativeHPBarPath;
	F_renderer = ren;
	F_x = x;
	F_y = y;
	init();
}
