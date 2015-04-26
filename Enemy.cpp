#include "Enemy.h"


void Enemy::E_render(){
	if (E_isVisible == true) {
		SDL_Rect clip;
		clip.x = 0; clip.y = 0; clip.w = 50; clip.h = 50;

		if (*E_isPaused == true){
			renderTexture(E_texture, E_renderer, E_x, E_y, &clip);
			renderTexture(E_HPBarTexture, E_renderer, E_x, E_y - 16, &E_HPBarClip[E_HP]);
			return void();
		}
		//przesuwanie
		if (E_x < E_moveToX)
			E_x++;
		else if (E_x > E_moveToX)
			E_x--;
		if (E_y < E_moveToY)
			E_y++;
		else if (E_y>E_moveToY)
			E_y--;

		renderTexture(E_texture, E_renderer, E_x, E_y, &clip);
		//HP Bar
		renderTexture(E_HPBarTexture, E_renderer, E_x, E_y - 16, &E_HPBarClip[E_HP]);
	}else if (E_isVisible==false){
			E_x = 0;
			E_y = 0;
			timer = 80;
			E_targetAcquired = false;
			return void();
		}
}


void Enemy::init(){
	E_texture = loadTexture(E_texturePath, E_renderer);
	E_HPBarTexture = loadTexture(E_HPBarPath, E_renderer);

	//HPBarClip
	int iterator = 0;
	for (int y = 0; y < 32; y += 16){
		for (int x = 0; x < 250; x += 50){
			E_HPBarClip[iterator].x = x;
			E_HPBarClip[iterator].y = y;
			E_HPBarClip[iterator].w = 50;
			E_HPBarClip[iterator].h = 16;
			iterator++;
		}
	}
}
Enemy::Enemy(){
	E_texturePath = ' ';
	E_renderer = NULL;
	E_x = 0;
	E_y = 0;
	E_HP = 10;
	E_isVisible = false;
}


Enemy::Enemy(string workingdir, SDL_Renderer *ren){
	E_isVisible = false;
	E_texturePath = workingdir + E_relativeTexturePath;
	E_HPBarPath = workingdir + E_relativeHPBarPath;
	E_renderer = ren;
	E_x = 0;
	E_y = 0;
	init();
}
Enemy::Enemy(string workingdir, SDL_Renderer *ren, int x, int y) {
	E_isVisible = false;
	E_texturePath = workingdir + E_relativeTexturePath;
	E_HPBarPath = workingdir + E_relativeHPBarPath;
	E_renderer = ren;
	E_x = x;
	E_y = y;
	init();
}
