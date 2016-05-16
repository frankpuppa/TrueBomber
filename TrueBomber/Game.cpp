#include "Game.h"
#include "Menu.h"
#include "Map.h"
#include "Player.h"
#include "TextureTTF.h"
#include "Sound.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

/*Game Constructor*/
Game::Game(){
   map=NULL;
   ingame=false;
   gameEnd=false;
   state_ok=false;
   quit=false;
   winner=0;
   timer=0;
   endedmatch=false;
}
Game::~Game(){
   std::cout<<"Cleaning Game Deconstructor"<<std::endl;
    free();
}
/*Function used to free and properly close all the initialized resources*/
void Game::free(){
 std::cout << "cleaning Game Class\n";
    freeMapPlayer();
    Sound::freeSounds();
    TTF_CloseFont( g_pFont );
    SDL_DestroyRenderer(g_pRenderer);
    g_pRenderer=NULL;
    SDL_DestroyWindow(g_pWindow);
    g_pWindow=NULL;
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}
/*Function used to initialize SDL libraries, background, music etc.. used within the game
 * If any of this will not be available the game should not start*/
bool Game::init(){
  
  if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){
        // if succeeded create our window
        g_pWindow = SDL_CreateWindow("True Bomber SDL2",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
        // if the window creation succeeded create our renderer
        if(g_pWindow == NULL){
            std::cout<<"Windows init fail\n";
            return false;
        }else{
            g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if(g_pRenderer==NULL){
                std::cout<<"Windows init fail\n";
                return false;
            }else{
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) ){
                    std::cout<<"SDL_image could not initialize! SDL_image Error: "<< IMG_GetError()<<"\n";
                    return false;
                }
                //if audio does not work, print a message, don't make all game unavailable
                Sound::init();
                //load all sounds used in the game
                Sound::loadSounds();

                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    return false;
                }

                if(!background.loadTexture("pics/walper.jpg", g_pRenderer)){
                    std::cout<<"Background cannot be loaded\n";
                    return false;
                }
               
                if(!Bombs.loadTexture("pics/bombs.png", g_pRenderer)){
                    std::cout<<"Bombs cannot be loaded\n";
                    return false;
                }
		
                if(!loadFont(0)){
                    std::cout<<"Font cannot be loaded\n";
                    return false;
                }
		if(!menu.init(&Bombs,&background,g_pFont,&input_handler,g_pRenderer,g_pWindow)){
		  std::cout<<"Menu failed init\n";
		  return false;
		}
            }
        }

    }else{
        std::cout<<"SDL_INIT cannot be initialized\n";
        return false; // sdl could not initialize
    }
    return true;
}
/*Game Loop Function. This is the core function of the game*/
void Game::gameLoop(bool *quit){

    *quit=false;
    
    Uint32 startTimer=SDL_GetTicks();

    while(!*quit && !gameEnd){
        gameEnd=input_handler.inputMap();

        for (unsigned int i=0; i<players.size(); i++){
            players.at(i)->move();
        }

        //std::cout<<"Collision detection"<< SDL_GetTicks()<<std::endl;
        menu.clearScreenRenderBG();
        //Show Map
        map->drawMap(g_pRenderer);
        map->checkBombs(g_pRenderer);
        //Show Player
        for(unsigned int i=0; i<players.size(); i++){
            players.at(i)->render(g_pRenderer);
        }
        checkGameState();
        // show the window
        SDL_RenderPresent(g_pRenderer);
        // set a delay before quitting
        if(1000/FPS>SDL_GetTicks()-startTimer){
            SDL_Delay(1000/FPS-(SDL_GetTicks()-startTimer));
        }
    }
}
/*This function open the font file and set the g_pFont pointer to the returned TTF type font*/
bool Game::loadFont(int size_text){
    if(size_text==0)
        size_text=28;
    //Loading success flag
    bool success = true;
    //Open the font
    g_pFont = TTF_OpenFont( "font/floating.ttf", size_text );
    if( g_pFont == NULL )
    {
        printf( "Failed to load Stack font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    return success;
}

void Game::run(){
  while(!quit){
    if(!state_ok){
      menu.displayMenu(&quit,&state_ok);
    }else{
      setupGame(&quit);
      gameLoop(&quit);
      checkGameEnd();
    }
  }
}
/*Function used to set up the game, free resource and allocate the new ones for next game*/
void Game::setupGame(bool*quit){
  
  if(*quit==true)
      return;
      
    freeMapPlayer();
    if(map==NULL){
        map=new Map();
    }
    menu.setupPlayers(map,&players);
    menu.setupMap(map,&players);
    gameEnd=false;
    ingame=false;
    endedmatch=false;
    
    
    if(!background.loadTexture("pics/background.jpg", g_pRenderer)){
        std::cout<<"Background cannot be loaded\n";
        *quit=true;
    }
    if(!input_handler.setPlayers(&players, menu.getPlayersControllerConfig())){
	*quit=true;
    }
    input_handler.setEndGame(&gameEnd);
    Sound::loadMusic("audio/fightmusic.wav");
    Sound::setMusicVolume(20);
    Sound::soundHandler(8);
   
}
/*This function check inside the gameLoop whenev the match is finish, meaning that either all players died or only one is still alive. 
 * The ended match variable guarantees that once the game is in the condition to end, this function is no longer executed. This avoid
 * the problem of entering this function another time and start another timer in a diferent thread, which would cause annoying problems*/
void Game::checkGameState(){
  
    if(endedmatch)
	return;
	
    unsigned int counter=0;
    winner=0;
    
    for(unsigned int i=0; i<players.size(); i++){
        if(players.at(i)->getDead()){
            counter++;
        }else{
	  winner=i+1;
	}
    }
    if(counter == players.size()){
      winner=0;
	endedmatch=true;
    }else if(counter ==players.size()-1){
	endedmatch=true;
    }
    if(endedmatch)
      startEndofGame();
}
void Game::startEndofGame(){
  for(unsigned int i=0; i<players.size(); i++){
    players.at(i)->stop_bombs();
  }
  timer=SDL_AddTimer(4500, delayEndGame, &gameEnd);
  ingame=true;
}
/*The following 3 functions are used by the timers when they expires - Will run on a separate thread*/
Uint32 Game::delayEndGame(Uint32 interval,void* game){

    *((bool*)game)=true;
    return 0;
}
/*This trick allow me to call a function from a static one*/
Uint32 Game::endofGame(Uint32 interval, void *game){
    ((Game*)game)->endofGame2(0);
      return 0;
}
Uint32 Game::endofGame2(Uint32 interval){
    menu.resetMenus();
    if(ingame==false)
        state_ok=false;
    gameEnd=false;
    return 0;
    
}
/*Function that displays end message*/
void Game::displayEnd(){
    SDL_Color textColor = {0,0,0,100};
    std::string message="";
    
    if(ingame){
      if(winner != 0){
	  message="  Winner is PLAYER " + menu.to_string(winner) + "  ";
      }else if(winner==0){
	  message="  There is no winner!  ";
      }
      menu.setupNextMap();
    }else{
      message="   Thank you for Playing   ";   
    }
    endTexture.loadFromRenderedText(message, textColor,g_pRenderer,g_pFont);
    SDL_TimerID timer=SDL_AddTimer(2000,endofGame,this);
    SDL_Rect r=endTexture.getTextureSize();
    while(gameEnd){
        menu.clearScreenRenderBG();
        endTexture.render(g_pRenderer, (SCREEN_WIDTH/2-(r.w/2)), (SCREEN_HEIGHT/2)-200);
        SDL_RenderPresent(g_pRenderer);

    }
    
}

void Game::freeMapPlayer(){
    if(players.size()!=0){
        for (unsigned int i=0; i<players.size(); i++){
            delete(players.at(i));
        }
    }
    players.clear();
    if(map!=NULL){
        delete(map);
        map=NULL;
    }
}
void Game::displayMenu(bool* q,bool *s){
  menu.displayMenu(q,s);
}
void Game::checkGameEnd(){
  if(gameEnd){
        Sound::stopMusic();
	Sound::freeMusic();
        displayEnd();
    }
}
