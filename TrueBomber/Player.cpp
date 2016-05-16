#include "Player.h"
#include "Bomb.h"
#include "Map.h"
#include "Sound.h"
#include <iostream>

/*** Player class ***/
/*Player Constructor*/
Player::Player(std::string name,Map *map){
    this->name=name;
    this->map=map;
    this->PLAYER_VEL=2;
    this->BOMB_NUM=1;
    this->POWER=1;
    this->FLAMEPWR=2;
    //flipType = SDL_FLIP_NONE;
    //this->direction=0;
    std::cout<<name + " created\n";
    canmove=true;
    dropbomb=false;
    spidertrap=true;
    this->bombs_number=BOMB_NUM;
    dead=false;
    stoned=false;
    kickbomb=false;
    previousTile=NULL;
    currentTile=NULL;
    cocaineTimer=0;
    spiderwebTimer=0;
    trapped=false;
    previousSpeed=0;
    mPosX=0;
    mPosY=0;
}
Player::~Player(){
    std::cout<<name +" deleted\n";
}
/*Function used to initialize the player features*/
bool Player::init(SDL_Renderer* g_pRenderer,std::string filename,std::string filexplosion, Texture * bombT, int bombPositioninFileX, int bombPositioninFileY)
{
    if(!texture.loadTexture(filename, g_pRenderer) || !explosionTexture.loadTextureMapColor(filexplosion,g_pRenderer,0,0,0)){
        std::cout<<"Error in one of the textures (Player class)\n";
        return false;
    }else{
        bombtexture=bombT;
        g_sourceR=texture.getTextureSize();
        g_sourceR.x=0;//FRAME_X;
        g_sourceR.y=FRAME_Y*3;

        g_destR.w=g_sourceR.w=FRAME_X;
        g_destR.h=g_sourceR.h=FRAME_Y;

        direction=0;

        //         explosionSrc=explosionTexture.getTextureSize();
        // explosionSrc.x=0;
        // explosionSrc.y=0;
        // explosionDst.w=40;
        // explosionDst.h=40;

        g_bombsrcR=bombtexture->getTextureSize();
        g_bombsrcR.x=bombPositioninFileX;
        g_bombsrcR.y=bombPositioninFileY; //40*4;
        g_bombsrcR.w=BOMBW;
        g_bombsrcR.h=BOMBH;

        return true;
    }
}
/*Function used to render player on screen*/
void Player::render(SDL_Renderer* g_pRenderer){
    if(!dead){
        g_destR.x=mPosX;
        g_destR.y=mPosY;
    }
    texture.render(g_pRenderer, &g_sourceR, &g_destR, 0.0,NULL, SDL_FLIP_NONE);
}
/* Function used to handle the movement of the player */
void Player::move(){
    if(dead || trapped){
        return;
    }

    int cellX=map->returnCellX(mPosX+PLAYEROFFSETX);
    int cellY=map->returnCellY(mPosY+PLAYEROFFSETY);
    currentTile=map->getTile(cellX,cellY);
    currentTile->setPlayeronTile(true);

    if(currentTile->getTrapPointer()){
        checkSetTrapped();
    }

    if(direction==1){
        checkCollision(cellX,cellY);
        if(canmove){
            mPosX+=PLAYER_VEL;
            currentTile=map->getTile(map->returnCellX(mPosX+PLAYEROFFSETX),map->returnCellY(mPosY+PLAYEROFFSETY));
            mPosY=currentTile->getmPosY()-(PLAYERH/2);

        }
        g_sourceR.y=FRAME_Y*3;
        g_sourceR.x=FRAME_X * int(((SDL_GetTicks() / 100) % 9));

    }else if(direction==-1){
        checkCollision(cellX,cellY);
        if(canmove){
            mPosX-=PLAYER_VEL;
            currentTile=map->getTile(map->returnCellX(mPosX+PLAYEROFFSETX),map->returnCellY(mPosY+PLAYEROFFSETY));
            mPosY=currentTile->getmPosY()-(PLAYERH/2);
            if(dropbomb){
                dropBomb();
            }
        }
        g_sourceR.y=FRAME_Y*1;
        g_sourceR.x=FRAME_X * int(((SDL_GetTicks() / 100) % 9));

    }else if(direction==2){
        checkCollision(cellX,cellY);
        if(canmove){
            mPosY -= PLAYER_VEL;
            currentTile=map->getTile(map->returnCellX(mPosX+PLAYEROFFSETX),map->returnCellY(mPosY+PLAYEROFFSETY));
            mPosX=currentTile->getmPosX();

        }
        g_sourceR.x=FRAME_X * int(((SDL_GetTicks() / 100) % 9));
        g_sourceR.y=FRAME_Y*2;


    }else if(direction==-2){
        checkCollision(cellX,cellY);
        if(canmove){
            mPosY+= PLAYER_VEL;
            currentTile=map->getTile(map->returnCellX(mPosX+PLAYEROFFSETX),map->returnCellY(mPosY+PLAYEROFFSETY));
            mPosX=currentTile->getmPosX();
        }
        g_sourceR.x=FRAME_X * int(((SDL_GetTicks() / 100) % 9));
        g_sourceR.y=0;
    }

    if(currentTile!=previousTile && previousTile!=NULL){
        //previousTile->setbomb(true);
        previousTile->setPlayeronTile(false);
        previousTile=NULL;
    }

    if(dropbomb){
        dropBomb();
    }

    if(currentTile->getItemType()!=0){
        pickUpItem();
    }
}

void Player::setPos(int x, int y){
    g_destR.x=mPosX=x;
    g_destR.y=mPosY=y;

}
int Player::getmPosX(){
    return mPosX;
}
int Player::getmPosY(){
    return mPosY;
}
/*Fuction used to check the player collision with all the rest nearby - Obviously if the player go against a bomb and it has the feature to kick the bomb, the bomb is set to travel*/
void Player:: checkCollision(int cellX, int cellY){


    if(currentTile!=NULL){
        previousTile=currentTile;
    }

    Tile (*tileptr)[COLUMN]=map->returngrid();
    Tile *tileTop, *tileBottom, *tileLeft, *tileRight;

    tileTop=&tileptr[cellY-1][cellX];
    tileBottom=&tileptr[cellY+1][cellX];
    tileLeft=&tileptr[cellY][cellX-1];
    tileRight=&tileptr[cellY][cellX+1];


    canmove=true;

    /*direction:*/
    //2=up -2=down -1=left 1=right


    switch(direction){
        case -1:
            if( ( tileLeft->getType()!=1 || tileLeft->hasbomb() )  && mPosX<=currentTile->getmPosX()   ){
                canmove=false;
                if(tileLeft->hasbomb()){
                    if(kickbomb){
                        tileLeft->getBombP()->setKicked(true);
                        tileLeft->getBombP()->setKickedState(direction);
                        tileLeft->getBombP()->setPlayerKicked(this);
                    }
                }
            }
            break;
        case 1:
            if((tileRight->getType()!=1 || tileRight->hasbomb() )&& mPosX>=currentTile->getmPosX() ){
                canmove=false;
                if(tileRight->hasbomb()){
                    if(kickbomb){
                        tileRight->getBombP()->setKicked(true);
                        tileRight->getBombP()->setKickedState(direction);
                        tileRight->getBombP()->setPlayerKicked(this);
                    }
                }
            }
            break;
        case 2:
            if( ( tileTop->getType()!=1 || tileTop->hasbomb() )&& mPosY+30<=currentTile->getmPosY() ){
                canmove=false;
                if(tileTop->hasbomb()){
                    if(kickbomb){
                        tileTop->getBombP()->setKicked(true);
                        tileTop->getBombP()->setKickedState(direction);
                        tileTop->getBombP()->setPlayerKicked(this);
                    }
                }
            }
            break;

        case -2:
            if( ( tileBottom->getType()!=1 || tileBottom->hasbomb() )&& mPosY+30>= currentTile->getmPosY()){
                canmove=false;
                if(tileBottom->hasbomb()){
                    if(kickbomb){
                        tileBottom->getBombP()->setKicked(true);
                        tileBottom->getBombP()->setKickedState(direction);
                        tileBottom->getBombP()->setPlayerKicked(this);
                    }
                }
            }
            break;
        default:
            break;
    }

}

Texture* Player::getExplosionT(){ return &explosionTexture;}
Texture* Player::getBombT(){ return bombtexture;}
int Player::getPower(){return POWER;}


/*This function is used to drop the bomb on screen. A new bomb object is created and when it explode, it will sed the tile to explosion. 
 * Data stored in the Tile is used to calculate the range of explosion in the Map class. When the explosion finishes the object bomb is retived and deleted from the map class.
 * Each tile can have only one bomb, and each player can drop a bomb only if the Tile's bomb pointer is NULL. This will prevent memory leakage in the game, as there should not be any loss 
 * in memory*/
void Player::dropBomb(){
    if(bombs_number!=0){
        int cellX,cellY;

        cellY=map->returnCellY(mPosY+PLAYEROFFSETY);
        cellX=map->returnCellX(mPosX+PLAYEROFFSETX);
        Tile *t=map->getTile(cellX,cellY);
        if(t->getBombP()==NULL){
            Sound::soundHandler(4);
            b=new Bomb(t->getmPosX(),t->getmPosY(),this,map);
            //std::cout<<"cellX: "<<cellX<<" cellY: "<<cellY<<std::endl;
            t->setbomb(true);
            t->setBombP(b);
            b->setTile(t);

            --bombs_number; //= (bombs_number==0)? 0 : bombs_number;
        }
    }
    dropbomb=false;
}
/*This function is responsible to pick up items from the map and play the respective sound. Different sounds are available according to the item picked up*/
void Player::pickUpItem(){

    switch(currentTile->getItemType()){
        case 1:
            Sound::soundHandler(2);
            increaseBOMB();
            break;
        case 2:
            Sound::soundHandler(2);
            increasePOWER();
            break;
        case 3:
            Sound::soundHandler(2);
            increaseVEL();
            break;
        case 4:
            Sound::soundHandler(2);
            kickbomb=true;
	    removeCocaineTimer();
            break;
        case 5:
            Sound::soundHandler(2);
            spidertrap=true;
	    removeCocaineTimer();
            break;
        case 6:
            Sound::soundHandler(7);
            stoned=true;
            break;
        case 7:
            Sound::soundHandler(2);
            increaseFLAME();
            break;
        case 8:
            Sound::soundHandler(6);
            underCOCAINE();
            break;
    }
    currentTile->setItemType(0);
}

/*The follosing function set the correct player movement. If the the player is under the influence of cannabinoids, the moves are inverted*/
void Player::move_down(){
    if(stoned){
        direction=2;
        return;
    }
    direction=-2;
}
void Player::move_up(){
    if(stoned){
        direction=-2;
        return;
    }
    direction=2;
}
void Player::move_left(){
    if(stoned){
        direction=1;
        return;
    }
    direction=-1;
}
void Player::move_right(){

    if(stoned){
        direction=-1;
        return;
    }
    direction=1;
}
void Player::drop_bomb(){
    dropbomb=true;
}
void Player::stop_move(){
    direction=0;
}
/*This function resent the number of bombs available to drop*/
void Player::reset_bombNum(){
    bombs_number=BOMB_NUM;
}
/*When player dies, this function will set the carachteristics of the player dead*/
void Player::setDead(bool d){
    dead=d;
    g_sourceR.x=360;
    g_sourceR.y=20;
    g_destR.x=currentTile->getmPosX();
    g_destR.y=currentTile->getmPosY();
    currentTile->setPlayeronTile(false);
}
/*The following functions change the bahaviour of the player when it picks up items- Some features like COCAINE have a temporary effect. For this reson, a timer is set. When the timer expires, the effect finishes.
 * Picking up other items, will remove the effect of the drug. If under cocaine the player gets the cannabis, both effects will happen at the same time.*/
bool Player::getDead(){
    return dead;
}
int Player::getFlamePwr(){
    return FLAMEPWR;
}
void Player::increaseBOMB(){
    if(BOMB_NUM<MAXBOMBS){
        BOMB_NUM++;
        bombs_number++;
    }
    removeCocaineTimer();
    stoned=false;
}
void Player::increaseVEL(){
    if(PLAYER_VEL<MAXVEL){
        PLAYER_VEL++;
    }
    removeCocaineTimer();
    stoned=false;
    //std::cout<<"Player VEL: "<<PLAYER_VEL<<std::endl;
}
void Player::increaseFLAME(){
    if(FLAMEPWR >0){
        FLAMEPWR--;
    }
    removeCocaineTimer();
    stoned=false;
}
void Player::increasePOWER(){
    if(POWER<=MAXPOWER){
        POWER++;
    }
    removeCocaineTimer();
    stoned=false;
}
void Player::underCOCAINE(){
    removeCocaineTimer();
    previousSpeed=PLAYER_VEL;
    PLAYER_VEL=MAXVEL*2;
    cocaineTimer = SDL_AddTimer( COCAINETIME, resetSpeed, this);
}
Uint32 Player::resetSpeed(Uint32 inteval,void* p){
    ((Player*) p)->resetSpeed();
    return 0;
}

Uint32 Player::resetSpider(Uint32 interval,void* tile){

    ((Tile *)tile)->getTrapPointer()->playerTrapped->releaseTrapped();
    ((Tile *)tile)->deleteTrapPointer();
    return 0;
}
SDL_Rect* Player::getBombSrc(){
    return &g_bombsrcR;
}
/*This function removes the cocaine timer. This guarantees that timer will keep running As then it will become problematic to deal with as they run on multiple threads*/
void Player::removeCocaineTimer(){
    if(cocaineTimer!=0){
        SDL_RemoveTimer(cocaineTimer);
        PLAYER_VEL=previousSpeed;
	cocaineTimer=0;
    }
}

void Player::installTrap(){
    if(!spidertrap){
        return;
    }else if(currentTile->getTrapPointer()==NULL){
        currentTile->installTrap(this);
        spidertrap=false;
    }
}
/*This function runs when the player gets trapped in another player trap -  This feature is exlusive for the True Bomber version :-) Obviously
 * the player is trapped temporarly, but this will allow another player to drop a bomb nearby*/
void Player::checkSetTrapped(){
    Trap *t=currentTile->getTrapPointer();
    if(t->playerSetTrap!=this && (mPosY+PLAYEROFFSETY > currentTile->getmPosY()+20 || mPosX +PLAYEROFFSETX > currentTile->getmPosX()+20)){
        Sound::soundHandler(5);
        t->playerTrapped=this;
        trapped=true;
        t->trapped=true;
        spiderwebTimer=SDL_AddTimer(SPIDERWEBTIME,resetSpider, currentTile);
    }
}
void Player::releaseTrapped(){
    trapped=false;
}
void Player::stop_bombs(){
  BOMB_NUM=0;
  bombs_number=0;
}
void Player::resetSpeed(){
  PLAYER_VEL=previousSpeed;
}
