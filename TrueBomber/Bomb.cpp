#include "Bomb.h"
#include "Tile.h"
#include "Player.h"
#include "Sound.h"
#include "Globals.h"
#include "Map.h"

#include <iostream>

/*** Bomb class ***/

Bomb::Bomb(int x,int y,Player* p,Map* map){
    std::cout<<"Bomb created"<<std::endl;
    player=p;
    kickedState=0;
    g_dest.x=x;
    g_dest.y=y;
    g_dest.w=g_dest.h=BOMBW;
    kicked=false;
    this->map=map;
    timerID = SDL_AddTimer( BOMBSECONDS * 1000, &Bomb::explosion, this);
}
Bomb::~Bomb(){
    std::cout<<"Bomb deleted\n";
    //SDL_RemoveTimer(timerID);

}

SDL_Rect* Bomb::getDstR(){
    return &g_dest;
}

void Bomb::setTile(Tile *t){
    tile=t;
}

Tile* Bomb::getTile(){ return tile;}


Player* Bomb::getPlayer(){return player;}



Uint32 Bomb::explosion(Uint32 interval, void* param){
    printf( "Callback called back with message: \n");
    return ((Bomb*)param)->explosion2(0);
}
Uint32 Bomb::explosion2(Uint32 interval){
    int bombCellX,bombCellY=0;
    switch(kickedState){
        case -1:
            bombCellX =map->returnCellX(g_dest.x);
            bombCellY=map->returnCellY(g_dest.y+(BOMBH/2));
            break;
        case 1:
            bombCellX =map->returnCellX(g_dest.x+BOMBW);
            bombCellY=map->returnCellY(g_dest.y+(BOMBH/2));
            break;
        case -2:
            bombCellX =map->returnCellX(g_dest.x+(BOMBW/2));
            bombCellY=map->returnCellY(g_dest.y+BOMBH);
            break;
        case 2: 
            bombCellX =map->returnCellX(g_dest.x+(BOMBW/2));
            bombCellY=map->returnCellY(g_dest.y);
            break;
        default:
            bombCellX =map->returnCellX(g_dest.x+(BOMBW/2));
            bombCellY=map->returnCellY(g_dest.y+(BOMBH/2));
            break;
    }

    tile=map->getTile(bombCellX,bombCellY);
    tile->setbomb(false);
    if(tile->getExplosion()==false){
        tile->setExplosion(true);
    }
    tile->setPlayer(player);
    //this->~Bomb();
    Sound::soundHandler(3);
    return interval;
}
void Bomb::setKickedState(int t){
    kickedState=t;
}
int Bomb::getKickedState(){
    return kickedState;
}
void Bomb::bombTravel(){
    switch(kickedState){
        case -1:
            g_dest.x-=BOMBSPEED;
            break;
        case 1:
            g_dest.x+=BOMBSPEED;
            break;
        case -2:
            g_dest.y+=BOMBSPEED;
            break;
        case 2:
            g_dest.y-=BOMBSPEED;
            break;
    }
}
void Bomb::travelBack(){
    switch(kickedState){
        case -1:
            g_dest.x+=BOMBSPEED;
            break;
        case 1:
            g_dest.x-=BOMBSPEED;
            break;
        case -2:
            g_dest.y-=BOMBSPEED;
            break;
        case 2:
            g_dest.y+=BOMBSPEED;
            break;
    }
}
int Bomb::getPosX(){
    return g_dest.x;
}
int Bomb::getPosY(){
    return g_dest.y;
}
bool Bomb::getKicked(){
    return kicked;
}
void Bomb::setKicked(bool t){
    kicked=t;
}

void Bomb::setPlayerKicked(Player *p){
    playerKicked=p;
}
Player* Bomb::getPlayerKicked(){
    return playerKicked;
}
SDL_TimerID Bomb::getTimer(){
    return timerID;
}

