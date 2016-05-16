#ifndef _PLAYER_H
#define _PLAYER_H

/*Forward Declarations*/
class Bomb;
class Map;
class Tile;

#include "Texture.h"



class Player
{
    public:
        static const int FRAME_X=40;
        static const int FRAME_Y=60;
        Player(std::string, Map *map);
        ~Player();
        void render(SDL_Renderer*);
        bool init(SDL_Renderer*, std::string, std::string, Texture*,int,int);
        void move();
        void setPos(int,int);
        int getmPosX();
        int getmPosY();

        void checkCollision(int ,int);
        Texture *getExplosionT();
        Texture *getBombT();
        void dropBomb();
        int getPower();
        void move_down();
        void move_up();
        void drop_bomb();
        void move_left();
        void move_right();
        void stop_move();
        void reset_bombNum();
        void setDead(bool);
        bool getDead();
        void pickUpItem();
        //increase Player's skills
        void increaseBOMB();
        void increaseVEL();
        void increaseFLAME();
        void increasePOWER();
        int getFlamePwr();
        void underCOCAINE();
        static Uint32 resetSpeed(Uint32,void*);
        static Uint32 resetSpider(Uint32,void*);
        SDL_Rect* getBombSrc();
        void removeCocaineTimer();
        void installTrap();
        void checkSetTrapped();
        void releaseTrapped();
	void stop_bombs();
	void resetSpeed();

    private:
        int PLAYER_VEL;
        int BOMB_NUM;
        int POWER;
        int FLAMEPWR;
        int previousSpeed;

        Texture texture;
        std::string name;
        SDL_Rect g_destR;
        SDL_Rect g_sourceR;
        Bomb *b;
        Map *map;
        Tile* previousTile;
        Tile * currentTile;

        int mPosX, mPosY;
        int direction;
        uint bombs_number;
        bool canmove;

        Texture explosionTexture;

        Texture *bombtexture;
        SDL_Rect g_bombsrcR;
        bool spidertrap;
        bool trapped;
        bool dropbomb;
        bool dead;
        bool stoned;
        bool kickbomb;
        SDL_TimerID cocaineTimer;
        SDL_TimerID spiderwebTimer;
};


#endif
