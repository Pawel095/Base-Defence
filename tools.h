#pragma once
#include <utility>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <math.h>

using namespace std;

struct Target{
	bool targetAcquired = false;
	int enemyNo;
	int timer = 80;
};

struct Button{
	SDL_Texture *texture;
	SDL_Rect dim;
};


/*
 * czysci wszelkie obiekty podane na liscie.
 */
template<typename T, typename... Args>
static void cleanup(T *t, Args&&... args){
	//czyœci pierwszy argument
	cleanup(t);
	//rekurencyjnie odpala nastepne czyszczenie
	cleanup(forward<Args>(args)...);
}

/*
 * to odpowiada za poprawnie wykonanie funkcji wyzej
 * ie. Specjalizacje funkcji
 */
template<>
void cleanup<SDL_Window>(SDL_Window *window){
	if (!window){
		return;
	}
	SDL_DestroyWindow(window);
}
template<>
void cleanup<SDL_Renderer>(SDL_Renderer *renderer){
	if (!renderer){
		return;
	}
	SDL_DestroyRenderer(renderer);
}
template<>
void cleanup<SDL_Texture>(SDL_Texture *texture){
	if (!texture){
		return;
	}
	SDL_DestroyTexture(texture);
}
template<>
void cleanup<SDL_Surface>(SDL_Surface *surface){
	if (!surface){
		return;
	}
	SDL_FreeSurface(surface);
}

//ulepszone loggowanie z opisem;

/**
*  loguje error ktory wywali SDL do wybranego strumienia wyjscia i zatrzymuje konsole aby dalo sie to przeczytac. (uwielbiam te nazwy)
*  @param os Strumien na ktory wiadomosc bedzie przekazana
*  @param msg Wiadaomosc o bledzie; format: "msg \n SDL_GetError()"
*/
static void logSDLError(ostream &os, string msg){
	os << msg << endl << SDL_GetError() << endl;
}

//ulepszone ³adowanie textur

/**
* laduje bmp na textue i odrazu do rendera
* @param file sciezka do pliku
* @param ren Renderer na ktory wczytac texture
* @return wczytana texture albo nullptr jak cos sie zepsuje
*/

static SDL_Texture *loadTexture(const string &file, SDL_Renderer *ren){

	//inicjalizacja do nullptr na wszelki wypadek
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(cout, string("loadTexture nie dziala."));
	}
	return texture;
}

//Funkcja renderujaca textury.


/**
* Rysuje SDL_TEXTURE kozystajac z SDL_RENDERER z pozycja (x, y)
* pozwalaj¹c na skalowanie
* @param tex textura do rysoannia
* @param ren Renderer do uzycia
* @param x coord x
* @param y coord y
* @param w szerokosc textury do narysowania
* @param h wysokosc textury do narysowania
*/
static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//twozy prostokot na wymaganej pozycji
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;

	rect.w = w;
	rect.h = h;
	SDL_RenderCopy(ren, tex, NULL, &rect);
}

/**
* Rysuje SDL_TEXTURE kozystajac z SDL_RENDERER z pozycja (x, y)
* zachowujac wysokosc i szerokosc textury
* @param tex textura do rysoannia
* @param ren Renderer do uzycia
* @param x coord x
* @param y coord y
*/
static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	//sprawdza jakie sa wys i szer textury.
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);

}

/**
* Rysuje SDL_TEXTURE kozystajac z SDL_RENDERER z pozycja (x, y)
* zachowujac wysokosc i szerokosc textury.
* jezeli damy clip (kordy fragmentu textury z wysokoscia i szerokoscia) zoztania uzyte jego WYS i SZER
* @param tex textura do narysowania
* @param ren renderer Renderujacy
* @param x coord x
* @param y coord y
* @param clip coordy fragmentu textury do narysowania
*/
static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,SDL_Rect *clip)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != NULL){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	SDL_RenderCopy(ren, tex, clip, &dst);
}


static void renderRotatedTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,double angle,SDL_Rect *clip)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != NULL){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	SDL_RenderCopyEx(ren,tex,clip,&dst,angle,NULL,SDL_FLIP_NONE);
}

//tylko dla mojego fonta. Ascii compatibile. working.
static SDL_Rect clips[256];
static bool clipsPrepped = false;
static void prepClips(){
	int fontW = 8, fontH = 8;
	int iterator = 0;
	for (int y = 0; y < 16; y++){
		for (int x = 0; x < 16; x++){
			clips[iterator].x = (x*fontW);
			clips[iterator].y = (y*fontW);
			clips[iterator].w = fontW;
			clips[iterator].h = fontH;
			iterator++;
		}
	}
	clipsPrepped = true;
}

static void renderWord(string word, int x, int y, SDL_Texture *font, SDL_Renderer *ren){

	if (clipsPrepped != true){
		prepClips();
	}

	const char *cword = word.c_str();
	int usefont = 4;

	for (unsigned int i = 0; i < word.length(); i++){
		usefont = *cword;
		renderTexture(font, ren, x, y, &clips[usefont]);
		cword++;
		x += 8;
	}
}

static void renderWord(unsigned int word, int x, int y, SDL_Texture *font, SDL_Renderer *ren){

	if (clipsPrepped != true){
		prepClips();
	}

	string stringWord = std::to_string(word);

	const char *cword = stringWord.c_str();
	int usefont = 4;

	for (unsigned int i = 0; i < stringWord.length(); i++){
		usefont = *cword;
		renderTexture(font, ren, x, y, &clips[usefont]);
		cword++;
		x += 8;
	}
}


static void renderWord(char tab[],int len, int x, int y, SDL_Texture *font, SDL_Renderer *ren){

	if (clipsPrepped != true){
		prepClips();
	}



	for (unsigned int i = 0; i<len; i++){
		renderTexture(font, ren, x, y, &clips[tab[i]]);
		x += 8;
	}
}


/**
* rysuje t³o na podstawie textur.
* @param ren Renderer do u¿ycia
* @param tex Textura do wyrenderowana
* @param SCREEN_WIDTH szerokozc okna
* @param SCREEN_HEIGHT wysokosc okna
*/
static void renderujTlo(SDL_Renderer *ren, SDL_Texture *tex, const int SCREEN_HEIGHT, const int SCREEN_WIDTH){
	int iW, iH;
	SDL_QueryTexture(tex, NULL, NULL, &iW, &iH);
	for (int i = 0; i < SCREEN_HEIGHT; i += iH){
		for (int j = 0; j < SCREEN_WIDTH; j += iW){
			renderTexture(tex, ren, j, i);
		}
	}
}

static double computeDistance(int x1, int y1, int x2, int y2){
	return sqrt(pow(y1-y2,2) + pow(x1-x2,2));
}

static bool isColiding(int leftA, int rightA, int topA, int bottomA,int leftB,int rightB,int topB,int bottomB){

	if (bottomA < topB){
		return false;
	}
	if (topA > bottomB){
		return false;
	}
	if (rightA < leftB){
		return false;
	}
	if (leftA > rightB){
		return false;
	}
	return true;
}

//nie dzia³a z ujemnym rect.w i rect.h
static bool isInside(SDL_Rect rect, int x, int y){
	if (rect.x < x && rect.y < y && (rect.x + rect.w)>x && (rect.y + rect.h)>y){
		return true;
	}
	return false;
}
static double getAngle(SDL_Rect a,SDL_Rect b){
    return abs(a.x-b.x)/abs(a.y-b.y);
}
