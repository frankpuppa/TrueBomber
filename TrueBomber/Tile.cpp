#include "Tile.h"
#include <iostream>

/*** Tile Class ***/
Tile:: Tile(){
    bombbool=false;
    type=1;
    explosion=false;
    counter=0;
    swaptile=false;
    endexplosion=false;
    explosionType=0;
    player=NULL;
    bombPtr=NULL;
    itemType=0;
    item=false;
    playeronTile=false;
    trapPtr=NULL;
    fire=false;
}
Tile::~Tile()
{
    if(trapPtr!=NULL){
        delete(trapPtr);
        std::cout<<"Delete Trap in Tile Decontructor"<<std::endl;
    }
}

void Tile::setbomb(bool b){ bombbool=b;}
int Tile::getmPosX(){return mPosX;}
int Tile::getmPosY(){return mPosY;}
int Tile::getType(){return type;}

bool Tile::hasbomb(){
    return bombbool;
}

void Tile::setType(int t){type=t;}

void Tile::setParameters(int x,int y,int w,int h, int t, Map * m)
{
    mPosX=x;
    mPosY=y;
    mWidth=w;
    mHeight=h;
    type=t;
    bombbool=false;
    map=m;
}

void Tile::setBombP(Bomb *b){
    bombPtr=b;
}
Bomb* Tile::getBombP(){
    return bombPtr;
}

bool Tile::getExplosion(){return explosion;}

void Tile::setExplosion(bool exps){ explosion=exps;}

//Map* Tile:: returnMap(){return map;}
//void Tile:: setMap(Map* m){map=m;}

/*To temporarly display explosions here I used CPU cycles rather than a timer. Maybe I should considering changing this. But for now it is fine..*/
void Tile::incCounter(){
    if (counter<35){
        counter++;
        // return false;
    }else{
        counter=0;
        //explosion=false;
        endexplosion=true;
    }
}

void Tile::setPlayer(Player * p){
    player=p;
}

Player *Tile::getPlayer(){
    return player;
}
void Tile::setswapTile(bool h){
    swaptile=h;
}
bool Tile::getSwap(){
    return swaptile;
}
int Tile::getCounter(){
    return counter;
}

bool Tile::explosion_finish(){
    return endexplosion;
}
void Tile::set_explosion_end(bool e){
    endexplosion=e;
}
int Tile::getExplosionType(){
    return explosionType;
}
void Tile::setExplosionType(int type){
    explosionType=type;
}
std::vector<Player*>* Tile::getPlayersExplosion(){
    return &playersEx;
}
void Tile::addPlayerEx(Player* p){
    playersEx.push_back(p);
}
/*These functions stores the explosions type and image related on each tile in a sequence- Basically the vectors behave like a stack.  Like already metion in the Map class this
 * was the only solution to draw the eplosions together wil the map. However this solution requires more memory and CPU usage. The thing is that for two player game, the vector won't fill up too much.
 * It works, but this solution is disgusting. However it enforces MVC*/
std::vector<int>* Tile::getPlayersExplosionType(){
    return &explosionTypeX;
}
void Tile::addExplosionType(int i){
    explosionTypeX.push_back(i);
}

int Tile::getItemType(){
    return itemType;
}
void Tile::setItemType(int t){
    itemType=t;
}

void Tile::setPlayeronTile(bool t){
    playeronTile=t;
}
bool Tile::tilehasPlayer(){
    return playeronTile;
}
/*This function allow the player to install a trap on the tile*/
void Tile::installTrap(Player* playerTrap){
    trapPtr=new Trap();
    trapPtr->playerSetTrap=playerTrap;
}
/*Return the pointer to the trap*/
Trap* Tile::getTrapPointer(){
    return trapPtr;
}
/*Deallocate resources used for the trap*/
void Tile::deleteTrapPointer(){
    if(trapPtr!=NULL){
        delete(trapPtr);
        std::cout<<"Delete Trap"<<std::endl;
        trapPtr=NULL;
    }
}
bool Tile::getFire(){
  return fire;
}
void Tile::setFire(bool f){
  fire=f;
}
