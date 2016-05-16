#ifndef _BOMB_H
#define _BOMB_H

class Tile;
class Player;
class Map;
#include <SDL2/SDL.h>

class Bomb
{
    public:
        Bomb(int,int, Player*,Map *);
        ~Bomb();
        SDL_Rect* getDstR();
        void setTile(Tile*);
        Tile* getTile();
        Player* getPlayer();
        static Uint32 explosion(Uint32, void*);
        Uint32 explosion2(Uint32);
        int getKickedState();
        void setKickedState(int);
        void bombTravel();
        void travelBack();
        int getPosX();
        int getPosY();
        bool getKicked();
        void setKicked(bool);
        Player* getPlayerKicked();
        void setPlayerKicked(Player *);
        SDL_TimerID getTimer();


    private:
        bool exploded;
        int kickedState;
        Tile* tile;
        SDL_Rect g_dest;
        //SDL_TimerID timerID;
        Player *player;
        Player *playerKicked;
        bool kicked;
        Map* map;
        SDL_TimerID timerID;
};
#endif
