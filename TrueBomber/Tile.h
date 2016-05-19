#ifndef _TILE_H
#define _TILE_H

class Player;
class Map;
class Bomb;
class Texture;
class Trap;

#include <SDL2/SDL.h>
#include<vector>




class Tile
{
    public:
        //Tile(int x=200,int y=200,int w=40,int h=40);
        //Tile(int,int,int,int, bool,int);
        Tile();
        ~Tile();
        void init();
        int getmPosX();
        int getmPosY();
        int getType();
        void setType(int);
        bool hasbomb();
        void setbomb(bool);

        void setParameters(int,int,int,int,int,Map *);
        void setPlayer(Player *);
        Player *getPlayer();

        void setBombP(Bomb *);
        Bomb* getBombP();
        bool getExplosion();
        void setExplosion(bool);
	
        void incCounter();
        void setswapTile(bool);
        bool getSwap();
        int getCounter();
        bool explosion_finish();
        void set_explosion_end(bool);
        void setExplosionType(int);
        int getExplosionType();

        std::vector<Player*>* getPlayersExplosion();
        void addPlayerEx(Player*);

        std::vector<int>* getPlayersExplosionType();
        void addExplosionType(int);

        void setItemType(int);
        int getItemType();
        void setPlayeronTile(bool);
        bool tilehasPlayer();
        void installTrap(Player*);
        void removeTrap();
        void deleteTrapPointer();
        Trap* getTrapPointer();
	void setFire(bool);
	bool getFire();

    private:
        int mPosX;
        int mPosY;
        int mWidth;
        int mHeight;
        bool bombbool;
        bool swaptile;
        bool endexplosion;
        bool item;
        Bomb* bombPtr;
	bool fire;
        bool explosion;
        int counter;
        Player *player;
        Map* map;
        int type;
        int explosionType;
        int itemType;
        std::vector<Player*> playersEx;
        std::vector<int> explosionTypeX;
        bool playeronTile;
        Trap * trapPtr;

};
struct Trap{
    Player* playerSetTrap;
    Player* playerTrapped;
    bool trapped;
};
#endif
