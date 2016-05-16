#ifndef _MAP_H
#define _MAP_H

class Player;

#include "Texture.h"
#include "Tile.h"
#include "Globals.h"
#include <vector>


class Map
{
    public:
        Map();
        ~Map();
        bool init(SDL_Renderer*,std::string, std::vector<Player*>*, std::string);
        bool loadMap();
        void drawMap(SDL_Renderer*);
        void render();
        int getTileX(int, int);
        int getTileY(int, int);
        
        Tile* getTile(int,int);
        int returnCellX(int);
        int returnCellY(int);
        Tile (*(returngrid)())[COLUMN];
        void checkNeighbour(int,int);

        void checkRangeEx(int,int);
        void handleExY(int,int, int, int, Player* );
        void handleExX(int,int, int, int, Player* );
        void checkBombs(SDL_Renderer*);
        void checkBoxesX(int,int,int);
        void checkBoxesY(int,int,int);
        void checkCollisionExplosionPlayer(int,int);
        int calculateItem();

        bool checkExistanceExplosionPlayer(Player*,int);
        void handleBombMovement(int, int, Bomb*);
        void handleBombMovementRight(int, int,Bomb *b);
        void handleBombMovementLeft(int, int,Bomb *b);
        void handleBombMovementUp(int, int, Bomb *b);
        void handleBombMovementDown(int, int,Bomb *b);
        bool bombCollidePlayer(Bomb*,Player*);


    private:
        Tile tileptr[ROW][COLUMN];
        Texture texture;
        Texture itemTexture;
        SDL_Rect g_sourceR;
        SDL_Rect g_destR;
        std::vector<Player*> *players;
        SDL_Rect explosionSrc;
        SDL_Rect explosionDst;
        SDL_Rect itemRectSrc; //<--ITEM
        Texture *explosionTexture;
        std::vector<Player*>* playersEx;
        std::vector<int>* playersExType;
        std::vector<Tile*>*tileEx;
        int TILESRC;
        std::string mapName;
};
#endif
