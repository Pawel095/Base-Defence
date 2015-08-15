#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <direct.h>
#include <stdio.h>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <SDL_mixer.h>

#include "tools.h"
#include "Friend.h"
#include "Enemy.h"
#include "Baza.h"
#include "Surowiec.h"

#define PI 3.14159265

#define F_NUMBER 5
#define E_NUMBER 20
#define P_BUFFER 50
#define S_BUFFER 3

#define E_SPAWNSPEED 150

using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;


int main(int, char**){
	//setup randomizera
	srand(time(NULL));
	//get working dir sie dzieje zeby BMP poprawnie wczytac.
	char char_CURRENT_PATH[FILENAME_MAX];
	if (!_getcwd(char_CURRENT_PATH, sizeof(char_CURRENT_PATH)))
	{
		logSDLError(cout, string("nie udalo sie odczytac katalogu programu!"));
		return 1;
	}
	cout << char_CURRENT_PATH << endl;
	string string_CURRENT_PATH(char_CURRENT_PATH);
	//ustawianie pathow do resources____________________________________________________________________________________PATH DO RESOURCES_________________________________________________________
	string dolImagePath = string_CURRENT_PATH + "\\RES\\TLO.jpg";
	string cursorImagePath = string_CURRENT_PATH + "\\RES\\cursor.png";
	string fontSpritePath = string_CURRENT_PATH + "\\RES\\font.png";
	string pauseButtonImagePath = string_CURRENT_PATH + "\\RES\\Pauza.png";
    string pociskImagePath = string_CURRENT_PATH + "\\RES\\pocisk.png";
    string unitiumImagePath = string_CURRENT_PATH + "\\RES\\Unitium.png";
    string repairCursorPath = string_CURRENT_PATH + "\\RES\\repairCursor.png";
    string repairButtonPath = string_CURRENT_PATH + "\\RES\\RepairMode.png";
    string OKbuttonPath = string_CURRENT_PATH + "\\RES\\OKbutton.png";
    string rangiPath = string_CURRENT_PATH + "\\RES\\Rangi.png";

	string gotItBossPath = string_CURRENT_PATH + "\\RES\\Sound\\Got it boss.wav";
	string acknowledgedPath = string_CURRENT_PATH + "\\RES\\Sound\\Acknowledged.wav";
	string movinToPositionPath = string_CURRENT_PATH + "\\RES\\Sound\\Movin to position.wav";
	string baseIsUnderAttackPath = string_CURRENT_PATH + "\\RES\\Sound\\Base is under attack.wav";
	string notEnoughtUnitiumPath = string_CURRENT_PATH + "\\RES\\Sound\\Not enought Unitium.wav";
	string maximumNumbersOfUnitsReached = string_CURRENT_PATH + "\\RES\\Sound\\maximum number of units reached.wav";
	string gameMusicPath = string_CURRENT_PATH + "\\RES\\Music\\Severe Tire Damage.mp3";
	string creditsMusicPath = string_CURRENT_PATH + "\\RES\\Music\\Two Finger Johnny.mp3";

	//Test czy sdl dzia³a________________________________________________________________________________________TESTY________________________________________________________________________________
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(cout, string("sdl nie dzia³a"));
		return 1;
	}

	//AUDIO!!!
	if (SDL_Init(SDL_INIT_AUDIO) != 0){
		logSDLError(cout, string("Audio sie zepsulo"));
		return 1;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) !=0){
		logSDLError(cout, string("Niedziala Mix_openAudio()"));
		return 1;
	}

	//ukrycie myszki
	SDL_ShowCursor(SDL_DISABLE);

	SDL_Window *okno = SDL_CreateWindow("Base Defense By Pawel Pankiewicz", 600, 400, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (okno == nullptr){
		logSDLError(cout, string("okno nie dzia³a"));
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(okno, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		logSDLError(cout, string("nie udalo sie stworzyc renderra"));
		SDL_Quit();
		return 1;
	}
	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);


	//setup variablesow_______________________________________________________________________________________________VAR SETUP_____________________________________________________________________
    //do zapisu hiscores-ow.################ODCZYT HISCORE##############
    fstream fileStream("scores.dat",ios::in | ios::out);
    string operatorFout;
    int hiScoresSize=0;
    //liczymy ilosc linijek.
    while(getline(fileStream,operatorFout)){
        //bo mamy 2 inputy na jedna linijke
        hiScoresSize+=2;
    }
    //przed ustawieniem seekg wyczysc flagi bledow.
    fileStream.clear();
    fileStream.seekg(0, ios::beg);
    string hiScoresAb[hiScoresSize];
    for(int i=0;i<hiScoresSize;i++){
        fileStream>>hiScoresAb[i];
    }
	SDL_Texture *tlo = loadTexture(dolImagePath, ren);
	SDL_Texture *cursor = loadTexture(cursorImagePath, ren);
	SDL_Texture *font = loadTexture(fontSpritePath, ren);
	SDL_Texture *pociskTexture = loadTexture(pociskImagePath,ren);
	SDL_Texture *surowceTexture = loadTexture(unitiumImagePath,ren);
	SDL_Texture *repairCursor = loadTexture(repairCursorPath,ren);
	SDL_Texture *repairButtonTexture = loadTexture(repairButtonPath,ren);
	SDL_Texture *rangi = loadTexture(rangiPath,ren);

	//button pauzy
	Button PauseButton;
	PauseButton.dim.x = WIDTH - 100; PauseButton.dim.y = 0; PauseButton.dim.h = 16; PauseButton.dim.w = 100;
	PauseButton.texture = loadTexture(pauseButtonImagePath, ren);
	//repairMode Button
    Button repairButton;
    repairButton.dim.x = WIDTH - 200; repairButton.dim.y = 0; repairButton.dim.h=16; repairButton.dim.w = 100;
    repairButton.texture = loadTexture(repairButtonPath,ren);
    //OK button
    Button OKbutton;
    OKbutton.dim.x=(WIDTH/2)-50; OKbutton.dim.y=HEIGHT/2+50; OKbutton.dim.h=50; OKbutton.dim.w=100;
    OKbutton.texture = loadTexture(OKbuttonPath,ren);

	//kursor
	int cursorW, cursorH;
	SDL_QueryTexture(cursor, NULL, NULL, &cursorW, &cursorH);
	int cursorX = (WIDTH / 2) - (cursorW / 2);
	int cursorY = (HEIGHT / 2) - (cursorH / 2);

	bool isRepairing = false;
	//punkty i surowce;
	int score = 0;
	int surowceCount=2;
	int unityCount=0;

	int difficulty=0;
	int awansCount=0;
	int partialDifficulty=0;
	char previousKey=0;
	string tempCzit;
	//pauza wszystkiego;
	bool pause = false;


	Pocisk pociski[P_BUFFER];
	for(int i=0; i<P_BUFFER; i++){
        pociski[i].P_x=0;
        pociski[i].P_y=0;
        pociski[i].isVisible=0;
        pociski[i].isPaused=&pause;
        pociski[i].P_moveToX=0;
        pociski[i].P_moveToY=0;
        pociski[i].P_ren=ren;
        pociski[i].P_tex=pociskTexture;
	}

	Friend Friends[F_NUMBER];
	for (int i = 0; i < F_NUMBER; i++){
		Friends[i].F_isSelected = false;
		Friends[i].F_isVisible = false;
		Friends[i].F_colides = false;
		Friends[i].F_renderer = ren;
		Friends[i].F_texturePath = string_CURRENT_PATH + Friends[i].F_relativeTexturePath;
		Friends[i].F_HPBarPath = string_CURRENT_PATH + Friends[i].F_relativeHPBarPath;
		Friends[i].F_rangi = loadTexture(rangiPath,ren);
		Friends[i].F_HP = 9;
		Friends[i].awansCount=&awansCount;
		Friends[i].F_isPaused = &pause;
		Friends[i].init();
	}

	Enemy Enemies[E_NUMBER];
	for (int i = 0; i < E_NUMBER; i++){
		Enemies[i].E_isVisible = false;
		Enemies[i].E_colides = false;
		Enemies[i].E_renderer = ren;
		Enemies[i].E_texturePath = string_CURRENT_PATH + Enemies[i].E_relativeTexturePath;
		Enemies[i].E_HPBarPath = string_CURRENT_PATH + Enemies[i].E_relativeHPBarPath;
		Enemies[i].E_HP = 3;
		Enemies[i].E_range = 100;
		Enemies[i].E_pointsPointer = &score;
		Enemies[i].E_isPaused = &pause;
		Enemies[i].init();
	}

	Surowiec surowce[S_BUFFER];
	for(int i =0;i<S_BUFFER; i++){
        surowce[i].tex=surowceTexture;
        surowce[i].ren=ren;
        SDL_QueryTexture(surowceTexture,NULL,NULL,&surowce[i].dim.w,&surowce[i].dim.h);
	}


	//zaznaczanie
	int DragTempX,DragTempY;
	bool isDragging = false;
	bool nickSceenDone=false;
    bool quit = false;
    SDL_Event e;


	Baza F_baza(string_CURRENT_PATH,ren,cursorX,cursorY);
	F_baza.B_HP = 99;
	F_baza.B_font = font;

	//audio setup, muzyka i efekty dzwiekowe.

	Mix_Chunk *gotItBoss = Mix_LoadWAV(gotItBossPath.c_str());
	Mix_Chunk *acknowledged = Mix_LoadWAV(acknowledgedPath.c_str());
	Mix_Chunk *movinToPosition = Mix_LoadWAV(movinToPositionPath.c_str());
	Mix_Chunk *baseIsUnderAttack = Mix_LoadWAV(baseIsUnderAttackPath.c_str());
	Mix_Chunk *notEnoughtUnitium = Mix_LoadWAV(notEnoughtUnitiumPath.c_str());
	Mix_Chunk *maximumNumberOfUnitsReached = Mix_LoadWAV(maximumNumbersOfUnitsReached.c_str());
	Mix_Music *gameMusic = Mix_LoadMUS(gameMusicPath.c_str());
	Mix_Music *creditsMusic = Mix_LoadMUS(creditsMusicPath.c_str());


    //EKRAN Z NICKIEM__________________________________________________________________NICK SCREEN__________________________________________________________________________________________________
    string EnterYourNickname="Enter Your Nickname. 30 chars max, no Uppercase supported (yet!)";
    char nickname[30]="";
    int iIterator=0;

    while(!nickSceenDone){
        if(iIterator<0)
            iIterator=0;
        else if(iIterator>29)
            iIterator=29;
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT){
                nickSceenDone=true;
                quit=true;
            }
            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_BACKSPACE){
                    iIterator--;
                    nickname[iIterator]=' ';
                }else if((e.key.keysym.sym>96 && e.key.keysym.sym<123) || (e.key.keysym.sym>47 && e.key.keysym.sym<58) ){
                    nickname[iIterator]=e.key.keysym.sym;
                    iIterator++;
                }
            }
            if(e.type==SDL_MOUSEMOTION){
                cursorX = e.motion.x;
				cursorY = e.motion.y;
            }
            if(e.type==SDL_MOUSEBUTTONDOWN){
                if(isInside(OKbutton.dim,cursorX,cursorY))
                    nickSceenDone=true;
            }
        }
        SDL_Rect rect; rect.x=0; rect.y=0; rect.w=WIDTH; rect.h=HEIGHT;
        SDL_RenderFillRect(ren,&rect);
        renderWord(EnterYourNickname,70, 200,font,ren);
        renderWord(nickname,29, (WIDTH/2)-30*4,(HEIGHT/2)-4, font,ren);
        renderTexture(OKbutton.texture,ren,OKbutton.dim.x,OKbutton.dim.y);
        renderTexture(cursor, ren, cursorX, cursorY);
        SDL_RenderPresent(ren);
    }

    //________________________________________________________________________________________TIMERY_________________________________________________________________________________________________
	int E_spawntimer = 0;
	int pociskTimer = 0;
	int surowceSpawnTimer = 0;
	SDL_RenderClear(ren);
	//________________________________________________________________________________________MAIN LOOP_______________________________________________________________________________________________
	//start muzyki
    Mix_PlayMusic(gameMusic,-1);
	while (!quit){
		SDL_Rect isDraggingRect;
		//eventy
		while (SDL_PollEvent(&e)){
			//zamkniecie okna
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_KEYDOWN){//____________________________________________________KLAWISZE___________________________________________________________________________________________________
				switch (e.key.keysym.sym){
				case SDLK_s:
				    if(isRepairing==false){
                        if(surowceCount>0 && unityCount<F_NUMBER){
                            surowceCount--;
                            for (int i = 0; i<F_NUMBER; i++){
                                if (Friends[i].F_isVisible == false){
                                    Friends[i].F_isVisible = true;
                                    Friends[i].F_x = cursorX-25;
                                    Friends[i].F_y = cursorY-25;
                                    Friends[i].F_moveToX = cursorX - 25;
                                    Friends[i].F_moveToY = cursorY - 25;
                                    break;
                                }
                            }
                        }else if(surowceCount>0){
                            if(!Mix_Playing(6)){
                                Mix_PlayChannel(6,maximumNumberOfUnitsReached,0);
                            }
                        }else{
                             if(!Mix_Playing(5)){
                                Mix_PlayChannel(5,notEnoughtUnitium,0);
                            }
                        }
				    }
					break;
				case SDLK_ESCAPE:
					for (int i = 0; i < F_NUMBER; i++){
						Friends[i].F_isSelected = false;
					}
					break;
				case SDLK_b:
                        F_baza.B_isVisible = true;
                        F_baza.B_x = cursorX;
                        F_baza.B_y = cursorY;
					break;
				}
				if(e.key.keysym.sym>96 && e.key.keysym.sym<123){
                    char tempkey=e.key.keysym.sym;
                    tempCzit+=tempkey;
                    cout<<tempkey<<endl;
				}
			}
			if(tempCzit.find(string("moarmonej")) !=string::npos ){
                surowceCount=99;
                tempCzit="";
			}if(tempCzit.length()>25565){
                tempCzit="";
			}


			if (e.type == SDL_MOUSEBUTTONDOWN){
				if (e.button.button == SDL_BUTTON_RIGHT && isRepairing==false){
					for (int i = 0; i < F_NUMBER; i++){
						if (Friends[i].F_isSelected == true){
							Friends[i].F_moveToX = cursorX;
							Friends[i].F_moveToY = cursorY;
						}
					}
					int los = rand() % 3;
					if(!Mix_Playing(3)){
                        switch (los){
                            case 0:
                                Mix_PlayChannel(3, acknowledged, 0);
                                break;
                            case 1:
                                Mix_PlayChannel(3, gotItBoss, 0);
                                break;
                            case 2:
                                Mix_PlayChannel(3,movinToPosition,0);
                            default:
                                break;
                        }
					}
				}
				if (e.button.button == SDL_BUTTON_LEFT){
					//klikniecie buttona pauzy;
					if (isInside(PauseButton.dim, cursorX, cursorY)){
						if (pause){
							pause = false;
						}else{
							pause = true;
						}
					}
					//klikniecie repairMode Button.
					if(isInside(repairButton.dim,cursorX,cursorY)){
                        if (isRepairing){
							isRepairing = false;
						}else{
							isRepairing = true;
						}
					}
					//klikniecie surowca.
					for(int i=0;i<S_BUFFER;i++){
                        if(isInside(surowce[i].dim,cursorX,cursorY) && isRepairing==false){
                            surowceCount++;
                            surowce[i].isVisible=false;
                        }
					}
					//draggowanie
					DragTempX = e.motion.x;
					DragTempY = e.motion.y;
					isDragging = true;
				}
			}else if(e.button.button == SDL_BUTTON_LEFT && isRepairing==true){
                //RepairMode#########################REPAIR MODE LOGIC######################
                SDL_Rect temp;
                //friendy
                for(int i=0;i<F_NUMBER;i++){
                    temp.x=Friends[i].F_x; temp.y=Friends[i].F_y; SDL_QueryTexture(Friends[i].F_texture,NULL,NULL,&temp.w,&temp.h);
                    if(isInside(temp,cursorX,cursorY) && Friends[i].F_HP<9 && surowceCount>0){
                        Friends[i].F_HP++;
                        surowceCount--;
                    }else if(surowceCount<=0){
                        if(!Mix_Playing(5)){
                            Mix_PlayChannel(5,notEnoughtUnitium,0);
                        }
                    }
                }
                //baza
                temp.x=F_baza.B_x; temp.y=F_baza.B_y; SDL_QueryTexture(F_baza.B_texture,NULL,NULL,&temp.w,&temp.h);
                if(isInside(temp,cursorX,cursorY) && F_baza.B_HP<99 && surowceCount>0){
                    F_baza.B_HP++;
                    surowceCount--;
                }else if(surowceCount<=0){
                    if(!Mix_Playing(5)){
                        Mix_PlayChannel(5,notEnoughtUnitium,0);
                    }
                }
            }
			if (e.type == SDL_MOUSEMOTION){
				cursorX = (e.motion.x);
				cursorY = (e.motion.y);
				//zaznaczanie;
				if (isDragging == true){
					isDraggingRect.x = DragTempX; isDraggingRect.y = DragTempY; isDraggingRect.w = cursorX - DragTempX; isDraggingRect.h = cursorY - DragTempY;
				}
			}
			if (e.type == SDL_MOUSEBUTTONUP){
				if (isDragging == true && isRepairing==false){
					for (int i = 0; i<F_NUMBER; i++){
						//+25 jest po to aby zaznaczac na srodku;
						if ((Friends[i].F_x+25 >DragTempX) && (Friends[i].F_y+25 >DragTempY) && (Friends[i].F_x+25 < cursorX) && (Friends[i].F_y+25 < cursorY)) {
							Friends[i].F_isSelected = true;
						}
					}
				}
				isDraggingRect.x = 0; isDraggingRect.y = 0; isDraggingRect.w = 0; isDraggingRect.h = 0;
				isDragging = false;
			}
		}
		//logika podnoszenia poziomu trudności._______________________________________DIFFICULTY LOGIC_____________________________________________________________________________________________
		if(partialDifficulty>7000){
            partialDifficulty=0;
            awansCount++;
            difficulty++;
		}else{
            partialDifficulty++;
		}

		//kolizje________________________________________________________________________LOGIKA UNITOW GRACZA____________________________________________________________________________________

		//sprawdzanie kolizji friendow
		for (int i = 0; i < F_NUMBER; i++){//g³ówny porównywany
			for (int j = i; j < F_NUMBER; j++){//do którego porównuje
				if (Friends[i].F_isVisible && Friends[j].F_isVisible && !(i==j) ){
					Friends[i].F_colides = isColiding(Friends[i].F_x, Friends[i].F_x + 50, Friends[i].F_y, Friends[i].F_y + 50, Friends[j].F_x, Friends[j].F_x + 50, Friends[j].F_y, Friends[j].F_y + 50);
					Friends[j].F_colides = Friends[i].F_colides;
				}
			}
		}
		//rozstawianie
		int countt = 0;
		int selected[F_NUMBER];
		for (int i = 0; i < F_NUMBER; i++){
			if (Friends[i].F_isSelected){
				selected[countt] = i;
				countt++;
			}
		}
        countt--;
		for (int i = 0; i < F_NUMBER; i++){
			if (countt >= 0 && Friends[i].F_colides){
				Friends[i].F_colides = false;

				switch (countt)
				{
				case 4:
					Friends[selected[4]].F_moveToY = Friends[selected[0]].F_y + 50;
				case 3:
					Friends[selected[3]].F_moveToX = Friends[selected[0]].F_x + 50;
				case 2:
					Friends[selected[2]].F_moveToX = Friends[selected[0]].F_x - 50;
				case 1:
					Friends[selected[1]].F_moveToY = Friends[selected[0]].F_y - 50;
				case 0:
					break;
				}
			}
		}
		//sprawdza czy w zasiegu frienda jest jakis enemy, startuje timer od obrazen i wystrzela pocisk.
		//stara sie proiorytetyzowac Enemiesow atakujace baze.
		for (int i = 0; i < F_NUMBER; i++){
			for (int j = 0; j < E_NUMBER; j++){
				if (Friends[i].range >= computeDistance(Friends[i].F_x, Friends[i].F_y, Enemies[j].E_x, Enemies[j].E_y) && Enemies[j].E_isVisible && Friends[i].F_isVisible){
                    int tempI=i;
					Friends[i].F_cel.enemyNo = j;
					Friends[i].F_cel.targetAcquired = true;

					/*//pociski */

					/* //pociski
					for(int i = 0;i<P_BUFFER;i++){
                        if(pociski[i].isVisible==false && pociskTimer>=100){
                            pociski[i].isVisible=true;
                            pociski[i].P_x=Friends[tempI].F_x;
                            pociski[i].P_y=Friends[tempI].F_y;
                            pociski[i].P_moveToX=Enemies[j].E_x;
                            pociski[i].P_moveToY=Enemies[j].E_y;
                            pociski[i].P_angle=atan(pociski[i].P_x/pociski[i].P_y)*180/PI;
                            pociski[i].licz();
                            cout<<"angle: "<<pociski[i].P_angle<<endl;
                            break;
                        }
					}
					*/
					break;
				}else{
					Friends[i].F_cel.targetAcquired = false;
				}
			}
		}
        //sprawdzanie priorytetu.
        for (int i = 0; i < F_NUMBER; i++){
            for (int j = 0; j < E_NUMBER; j++){
                if (Friends[i].range >= computeDistance(Friends[i].F_x, Friends[i].F_y, Enemies[j].E_x, Enemies[j].E_y) && Enemies[j].E_isVisible && Friends[i].F_isVisible && Enemies[j].E_targetAcquired==true){
                    Friends[i].F_cel.enemyNo = j;
                    Friends[i].F_cel.targetAcquired = true;
                    break;
                }
            }
        }
        //zlicza aktywne jednostki
        unityCount=0;
        for(int i=0; i<F_NUMBER;i++){
            if(Friends[i].F_isVisible==true)
                unityCount++;
        }

		//_________________________________________________________________________________________LOGIKA PRZECIWNIKÓW_________________________________________________________________________________


        //wykrywanie unita gracza.

        for(int i=0;i<E_NUMBER;i++){
            for(int j=0;j<F_NUMBER;j++){
                if(Enemies[i].E_range>=computeDistance(Enemies[i].E_x,Enemies[i].E_y,Friends[j].F_x,Friends[j].F_y) && Enemies[i].E_isVisible==true && Friends[j].F_isVisible==true){
                    Enemies[i].E_targetAcquired=true;
                    Enemies[i].E_targetNo=j;
                }
            }
        }
        //strzal E-> baza.#####PRIORYTETEM JEST BAZA!!!
		for (int i = 0; i < E_NUMBER; i++){
			if (Enemies[i].E_range >= computeDistance(Enemies[i].E_x, Enemies[i].E_y, F_baza.B_x, F_baza.B_y)){
                Enemies[i].E_targetNo=123;
				Enemies[i].E_targetAcquired = true;
			}
		}


		//coklatkowy render______________________________________________________________________________RENDER________________________________________________________________________________________

		renderujTlo(ren, tlo, HEIGHT, WIDTH);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
		if(isRepairing==false){
            SDL_RenderDrawRect(ren, &isDraggingRect);
		}

		//renderowanie friendów, numerów
		for (int i = 0; i < F_NUMBER; i++){
			Friends[i].F_render();
			renderWord(i, Friends[i].F_x, Friends[i].F_y, font, ren);
		}

		//render pociskow
		for(int i = 0; i<P_BUFFER; i++){
            if(pociski[i].isVisible==true){
                pociski[i].render();
            }
		}

		for(int i = 0; i<S_BUFFER; i++){
            if(surowce[i].isVisible==true){
                surowce[i].render();
            }
		}

		//baza
		F_baza.B_render();

		//render enemiesow
		for (int i = 0; i < E_NUMBER; i++){
			Enemies[i].E_render();
		}

		//##########render Menu u góry;##############
		//tlo
		SDL_Rect menuRect;
		menuRect.x = 0; menuRect.y = 0; menuRect.h = 16; menuRect.w = WIDTH;
		SDL_RenderFillRect(ren, &menuRect);
		//wynik;
		renderWord(string(" Score: " + std::to_string(score)), 0, 5, font, ren);
		//ilosc surowcow
		//jednosci
		char surowce1=48+surowceCount % 10;
		//dziesiatki
		char surowce2=48+(surowceCount/10) % 10;
		char tab[10]={'A','m','o','u','n','t',1,':',surowce2,surowce1};
		renderWord(tab,10,WIDTH / 4, 5, font, ren);
		//supply
		char unitCstr[5]={2,':',48+unityCount % 10,'/','5'};
		renderWord(unitCstr,5,WIDTH / 2,5,font,ren);
		//button pauzy
		renderTexture(PauseButton.texture, ren, PauseButton.dim.x, PauseButton.dim.y);
		//button repairmode.
		renderTexture(repairButton.texture,ren,repairButton.dim.x,repairButton.dim.y);

		//jezeli jakis enemies jest namierzony to rysuje kreske F->E
		for (int i = 0; i < F_NUMBER; i++){
			if (Friends[i].F_cel.targetAcquired && Friends[i].F_cel.timer <= 80){
				SDL_RenderDrawLine(ren, Friends[i].F_x, Friends[i].F_y, Enemies[Friends[i].F_cel.enemyNo].E_x, Enemies[Friends[i].F_cel.enemyNo].E_y);
			}
		}

		//enemies namierza baze/Frienda i rysuje kreske;
		for (int i = 0; i < E_NUMBER; i++){
			if (Enemies[i].E_targetAcquired && Enemies[i].timer <= 80){
                if(Enemies[i].E_targetNo==123){
                    SDL_RenderDrawLine(ren, Enemies[i].E_x, Enemies[i].E_y, F_baza.B_x, F_baza.B_y);
                }else{
                    SDL_RenderDrawLine(ren,Enemies[i].E_x,Enemies[i].E_y,Friends[Enemies[i].E_targetNo].F_x,Friends[Enemies[i].E_targetNo].F_y);
                }
			}
		}

		//kursor;
		if(isRepairing==false){
            renderTexture(cursor, ren, cursorX, cursorY);
		}else{
            renderTexture(repairCursor, ren, cursorX, cursorY);
		}
        SDL_RenderPresent(ren);
        //______________________________________________________________________________STEROWANIE DZWIEKIEM_______________________________________________________________________________________

        if(pause){
            Mix_Pause(-1);
            Mix_PauseMusic();
        }else{
            Mix_Resume(-1);
            Mix_ResumeMusic();
        }
		//________________________________________________________________________________SPAWNOWANIE I RESET TIMEROW______________________________________________________________________________
		if (pause == false){
			//spawner surowców.
			if(surowceSpawnTimer<500){
                surowceSpawnTimer++;
			}else if(F_baza.B_isVisible){
                for(int i=0;i<S_BUFFER;i++){
                    if(surowce[i].isVisible==false){
                        surowce[i].isVisible=true;
                        surowce[i].dim.x=30+rand() % (WIDTH-60);
                        surowce[i].dim.y=30+rand() % (HEIGHT-60);
                        surowceSpawnTimer=0;
                        break;
                    }
                }
			}
			//spawn Enemiesow
			if (E_spawntimer <= E_SPAWNSPEED - (difficulty*3)){
				E_spawntimer++;
			}
			else{
				E_spawntimer = 0;
				for (int i = 0; i < E_NUMBER; i++){
					if (Enemies[i].E_isVisible == false && F_baza.B_isVisible){
						Enemies[i].E_isVisible = true;
						int gdzie = rand() % 4;

						switch (gdzie)
						{
						case 0:
							//top
							Enemies[i].E_x = rand() % (WIDTH + 1);
							Enemies[i].E_y = -50;
							break;
						case 1:
							//left
							Enemies[i].E_x = WIDTH + 50;
							Enemies[i].E_y = rand() % (HEIGHT + 1);
							break;
						case 2:
							//right
							Enemies[i].E_x = -50;
							Enemies[i].E_y = rand() % (HEIGHT + 1);
							break;
						case 3:
							//down
							Enemies[i].E_x = rand() % (WIDTH + 1);
							Enemies[i].E_y = HEIGHT + 50;
							break;
						}

						Enemies[i].E_moveToX = F_baza.B_x;
						Enemies[i].E_moveToY = F_baza.B_y;
						break;
					}
				}
			}
			//timer od strzelania friendow
			for (int i = 0; i < F_NUMBER; i++){
				if (Friends[i].F_cel.targetAcquired && Friends[i].F_cel.timer <= 80){
					Friends[i].F_cel.timer++;
				}
				else if (Friends[i].F_cel.targetAcquired){
					Friends[i].F_cel.timer = 0;
					Enemies[Friends[i].F_cel.enemyNo].E_HP-=Friends[i].ranga+1;

					if(Enemies[Friends[i].F_cel.enemyNo].E_HP<=-1 && awansCount<7){
                        Friends[i].kills++;
                        score++;
                        Enemies[Friends[i].F_cel.enemyNo].E_isVisible=false;
                        Enemies[Friends[i].F_cel.enemyNo].E_HP=awansCount+3;
					}else if(Enemies[Friends[i].F_cel.enemyNo].E_HP<=-1){
                        Friends[i].kills++;
                        score++;
                        Enemies[Friends[i].F_cel.enemyNo].E_isVisible=false;
                        Enemies[Friends[i].F_cel.enemyNo].E_HP=9;
					}
				}
			}
			//timer od spawnowania pociskow;  #####NIE DZIALA####
			if(pociskTimer>=100){
                pociskTimer=0;
			}else{
                pociskTimer++;
			}

			//timer od strzelania enemiesów;
            for (int i = 0; i < E_NUMBER; i++){
                if (Enemies[i].E_targetAcquired && Enemies[i].timer <= 80){
                    Enemies[i].timer++;
                }else if (Enemies[i].E_targetAcquired){
                    Enemies[i].timer = 0;
                    if(Enemies[i].E_targetNo==123){
                        if(awansCount>7 && awansCount<=13){
                            F_baza.B_HP-=awansCount-5;
                        }else if(awansCount>13){
                            F_baza.B_HP-=10;
                        }
                        else{
                            F_baza.B_HP--;
                        }
                        if(!Mix_Playing(4)){
                             Mix_PlayChannel(4,baseIsUnderAttack,0);
                        }
                    }else{
                         if(awansCount>7){
                            Friends[Enemies[i].E_targetNo].F_HP-=awansCount-5;
                        }else if(awansCount>13){
                            Friends[Enemies[i].E_targetNo].F_HP-=10;
                        }
                        else{
                            Friends[Enemies[i].E_targetNo].F_HP--;
                        }
                        if(Friends[Enemies[i].E_targetNo].F_HP<=-1){
                            Friends[Enemies[i].E_targetNo].F_isVisible=false;
                            Friends[Enemies[i].E_targetNo].F_HP=9;
                            Friends[Enemies[i].E_targetNo].ranga=0;
                            Friends[Enemies[i].E_targetNo].kills=0;
                        }
                    }
                }
            }
            system("cls");
            //logging
            cout<<tempCzit<<endl;
            cout<<awansCount<<" awansCount"<<endl;
		}

		//___________________________________________________________________________________WERBE POPROSZE! WARUNEK PRZEGRANIA TU JEST!___________________________________________________________________________

		if (F_baza.B_HP<0){
			string message = string("You have lost! Score: ") + to_string(score);
			MessageBox(0, message.c_str(), "Game Over!", MB_OK);
			quit = true;
		}
	}
	//tutaj zapis do pliku\
	##########ZAPIS HISCORE##############
	fileStream.clear();
	operatorFout = string(nickname)+" "+std::to_string(score);
	fileStream<<operatorFout<<endl;
    fileStream.close();
    //creditz mode!_______________________________________________________________________CREDITS___________________________________________________________________________________________________________________________

    Mix_HaltMusic();
    Mix_PlayMusic(creditsMusic,0);
    bool done=false;
    int x=8,y=HEIGHT;
    while(!done){
            SDL_RenderClear(ren);
        if(y < -158){
            done=true;
        }
        SDL_PollEvent(&e);
        renderWord(string("\"Severe Tire Damage\", \"Two Finger Johnny\""),x,y,font,ren);
        renderWord(string("Kevin MacLeod (incompetech.com)"),x,y+9,font,ren);
        renderWord(string("Licensed under Creative Commons: By Attribution 3.0"),x,y+18,font,ren);
        renderWord(string("http://creativecommons.org/licenses/by/3.0/"),x,y+27,font,ren);
        renderWord(string("Main programmer: Pawel Pankiewicz"),x,y+50,font,ren);
        renderWord(string("Graphic designer: Pawel Pankiewicz"),x,y+76,font,ren);
        renderWord(string("Voice actor: Pawel Pankiewicz"),x,y+102,font,ren);

        y--;
		SDL_RenderPresent(ren);
    }

	//czyszczenie uzupelnic!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DOKONCZ CZYSZCZENIE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	fileStream.close();
	return 0;
}
