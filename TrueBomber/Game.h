#ifndef _GAME_H
#define _GAME_H
#include "Menu.h"

class Game{
    
    public:
      Game();
      ~Game();
      void gameLoop(bool*);
      void setRenderer(SDL_Window*);
      void setWindow(SDL_Renderer*);
      void free();
      bool init();
      void run();
      bool loadFont(int size_text=0);
      void setupGame(bool*);
      void checkGameState();
      static Uint32 delayEndGame(Uint32,void*);
      void freeMapPlayer();
      static Uint32 endofGame(Uint32,void*);
      Uint32 endofGame2(Uint32);
      void displayEnd();
      void displayMenu(bool*,bool*);
      void startEndofGame();
      void checkGameEnd();
    
    private:
      SDL_Window* g_pWindow;
      SDL_Renderer* g_pRenderer;
      Menu menu;
      Texture background;
      Texture Bombs;
      TTF_Font *g_pFont;
      InputHandler input_handler;
      std::vector<Player*> players;
      Map *map;
      bool ingame;
      bool gameEnd;
      TextureTTF endTexture;
      bool state_ok;
      bool quit;
      int winner;
      bool endedmatch;
      
      SDL_TimerID timer;
};

#endif //_GAME_H
