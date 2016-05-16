#ifndef _MENU_H
#define _MENU_H

class Map;
class Player;
class TextureTTF;
//class Texture;
#include "Texture.h"
#include "TextureTTF.h"
#include "InputHandler.h"
#include <vector>
#include "MapHandler.h"


class Menu{

    public:
        Menu();
        ~Menu();
        bool init(Texture*,Texture*, TTF_Font*, InputHandler*,SDL_Renderer*,SDL_Window*);
        void displayMenu(bool*,bool*);
        void free();
        void handleUP();
        void handleDOWN();
        void handleRIGHT();
        void handleLEFT();
        void handleRETURN();
        void handleESCAPE();
        void setupMenu();
        void menuSelect();
        void menuEscape();
        void renderCharacterChoice();
        void initTextures();
        void freeDynamicMemory();
        template<typename T>std::string to_string(T);
        void setupMap(Map*,std::vector<Player*>*);
        void setupPlayers(Map*,std::vector<Player*>*);
        void freeSounds();
        void handleMenus();
        void resetMenus();
	void setupNextMap();
        void clearScreenRenderBG();
	void setupPlayersTTF();
	void freePlayersTTF();
	void freeControllers();
	void handleMusic();
	void displayRectMenu();
	void displayControllers();
	void setupControllers();
	std::vector<int>* getPlayersControllerConfig();
	
    private:
        SDL_Window* g_pWindow;
        SDL_Renderer* g_pRenderer;
        //std::string const menusText[3];
        static const std::string menusText[3];
        static const std::string menusText2[2];
        static const std::string menuTitles[6];
	static const std::string audioText[2];
	
        std::vector<std::string> menuEntryVect;
        std::vector<std::string> maps;

        std::string const *menuEntryPtr;
	std::vector<int> playerControllerConfig;

        bool *quit;
        bool *state_ok;

        bool menu1;
        bool menu2;
        bool menu3;
        bool menu4;
	bool menu5;
	bool menu6;

        Texture *background;
        Texture Players;
        Texture Explosions;
        Texture *Bombs;
	Texture ControllersT;

        TextureTTF menuTitle;
        TextureTTF menuEntry;	
	
	TextureTTF *playersTTF; //menu4 textures
	
        bool unavailable;
        
        SDL_Rect *Expsrc;
        SDL_Rect *Expdest;
        SDL_Rect *Plyrsrc;
        SDL_Rect *Plyrdest;
        SDL_Rect *Bombsrc;
        SDL_Rect *Bombdest;
	
	SDL_Rect *ControllerSourceR;
	SDL_Rect *ControllerDestR;
        
	TTF_Font *g_pFont;

        SDL_Rect *pointerRect;

        //stores number of players
        int player_number;
        int *indexPlayer;
        int *indexExplosion;
        int *indexBomb;
        InputHandler* input_handler;
	MapHandler mapfilesHandler;
};


#endif // MENU_H_INCLUDED
