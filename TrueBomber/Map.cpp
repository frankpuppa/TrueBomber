#include "Map.h"
#include "Bomb.h"
#include "Player.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>


/*** Map Class ***/
Map::Map()
{
    std::cout<<"Map created\n";
    explosionDst.w=explosionDst.h=40;
    explosionSrc.w=explosionSrc.h=40;
    playersEx=NULL;
    playersExType=NULL;
    tileEx=NULL;
}
Map::~Map()
{
    std::cout<<"Map deleted\n";
    std::cout<<"delete tiles: \n";
}
/*This function initializes the object Map. So, all the textures require to displays various objects of the map*/
bool Map::init(SDL_Renderer* g_pRenderer, std::string filename, std::vector<Player*> *players, std::string mapName)
{
    if(!texture.loadTexture(filename, g_pRenderer) || !itemTexture.loadTexture("pics/items.png",g_pRenderer) ){
        std::cout<<"Error loading either map or items texture\n";
        return false;
    }else{
        g_sourceR=texture.getTextureSize();
        g_sourceR.x=0;  g_destR.x=60;
        g_sourceR.y=0;  g_destR.y=40;
        g_destR.w=g_sourceR.w=40;
        g_destR.h=g_sourceR.h=40;
        itemRectSrc=itemTexture.getTextureSize();
        itemRectSrc.w=itemRectSrc.h=TILESIZE;
	TILESRC=(rand() % 6) *160;
        this->mapName="maps/" + mapName+ ".map";
        this->players=players;
        if(loadMap())
            return true;
        else
            return false;
    }
}
/*This map draws the map on screen, I tried to enforce the fact that the map should be responsible for displaying everythng on top,
 * and not oly the map. However, I'm not entirely sure this solution is. Definitely more MVC that in the other case.*/
void Map::drawMap(SDL_Renderer* g_pRenderer) //1=back 2=wall 3=box 4=explosion box
{
    for(int l=0; l<ROW*COLUMN; l++){
        int j=l % (GRIDWIDTH/TILESIZE);
        int i=l / (GRIDWIDTH/TILESIZE);
        g_sourceR.x=TILESRC; //position in the maptiles.png file
        if(tileptr[i][j].getType()==1)
            g_sourceR.x=TILESRC;
        else if(tileptr[i][j].getType()==2)
            g_sourceR.x+=TILESIZE;
        else if(tileptr[i][j].getType()==3)
            g_sourceR.x+=TILESIZE*2;
        else if(tileptr[i][j].getType()==4)
            g_sourceR.x+=TILESIZE*3;
        else
            continue;

        g_destR.x=getTileX(i,j);
        g_destR.y=getTileY(i,j);


        texture.render(g_pRenderer, &g_sourceR, &g_destR);

        if(tileptr[i][j].tilehasPlayer() ){

            SDL_SetRenderDrawBlendMode(g_pRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 255, 50);
            SDL_RenderFillRect(g_pRenderer, &g_destR);
        }
        if(tileptr[i][j].getTrapPointer()!=NULL){
            if(tileptr[i][j].getTrapPointer()->trapped){
                g_sourceR.x=960;
                texture.render(g_pRenderer, &g_sourceR, &g_destR);
            }
        }
        if(tileptr[i][j].getItemType()!=0){
            switch(tileptr[i][j].getItemType()){
                case 1:
                    itemRectSrc.x=0;
                    break;
                case 2:
                    itemRectSrc.x=ITEMSIZE;
                    break;
                case 3:
                    itemRectSrc.x=ITEMSIZE*2;
                    break;
                case 4:
                    itemRectSrc.x=ITEMSIZE*3;
                    break;
                case 5:
                    itemRectSrc.x=ITEMSIZE*4;
                    break;
                case 6:
                    itemRectSrc.x=ITEMSIZE*5;
                    break;
                case 7:
                    itemRectSrc.x=ITEMSIZE*6;
                    break;
                case 8:
                    itemRectSrc.x=ITEMSIZE*7;
                    break;
            }
            itemTexture.render(g_pRenderer,&itemRectSrc,&g_destR);
        }

        playersEx=tileptr[i][j].getPlayersExplosion();
        playersExType=tileptr[i][j].getPlayersExplosionType();
        if(playersEx->size()!=0){
            int typeExP=0;
            Player* pl=NULL;

            explosionDst.x=tileptr[i][j].getmPosX();
            explosionDst.y=tileptr[i][j].getmPosY();
            for(unsigned int k=0; k<playersEx->size(); k++){
                pl=playersEx->at(k);
                explosionTexture=pl->getExplosionT();
                explosionSrc.y=pl->getFlamePwr()*EXPLOSIONH;
                typeExP=playersExType->at(k);
                switch(typeExP){
                    case -1:
                        explosionSrc.x=0;
                        break;
                    case 1:
                        explosionSrc.x=80;
                        break;
                    case -2:
                        explosionSrc.x=200;
                        break;
                    case 2:
                        explosionSrc.x=120;
                        break;
                    case 3:
                        explosionSrc.x=240;
                        break;
                    case 4:
                        explosionSrc.x=160;
                        break;
                    case 5:
                        explosionSrc.x=40;
                        break;
                    default:
                        break;
                }
                explosionTexture->render(g_pRenderer,&explosionSrc,&explosionDst,0.0,NULL,SDL_FLIP_NONE);
            }
        }
  }
}
/*This function load the map from the txt file and set all the required thing, such a player position on the map, Tile type etc..*/
bool Map::loadMap(){
    std::string line;
    //int nplayer=0; //number of players
    int x=0;
    int y=0;

    int tilex=60; //offset for X axes
    int tiley=40;// offset for Y axes

    std::ifstream reader(mapName.c_str(), std::ifstream::in);
    if(!reader.is_open()){
        std::cout<<"Error, can't read file\n";
        return false;
    }
    int index_vector=0;
    while(getline(reader, line)){

        //int player_number=players->size();
        if(line.length()<2){
            //std::cout<<(int)line.c_str()<<std::endl;
            //nplayer=atoi(line.c_str());
            //        nplayer=*(line.c_str())-'0';
            continue;
        }else{
            x=0;
            tilex=60; // xoffset when drawing the map
            for(uint i=0; i<line.length(); i++){
                if(isdigit(line.at(i))){
                    uint n=(int)line.at(i) - '0';  //fast way to convert a char into int
                    if(n<players->size())
                        switch(n){
                            case 0:
				if(players->at(n)->getmPosX() == 0){
				  players->at(n)->setPos(tilex,tiley-(PLAYERH/2));
				  index_vector++;
				}
                                break;
                            case 1:
				  if(players->at(n)->getmPosX() == 0){
				    players->at(n)->setPos(tilex,tiley-(PLAYERH/2));
				    index_vector++;
				  }
                                break;
                        }
                    line.at(i)=' ';
                }

                switch(line.at(i)){
                    //wall
                    case '*': tileptr[y][x].setParameters(tilex,tiley,40,40,2,this);
                              //tileptr[y][x].setMap(this);
                              break;   //int type 1=back 2=wall 3=box 4=explosion box
                              //box
                    case '+': tileptr[y][x].setParameters(tilex,tiley,40,40,3,this);
                              //tileptr[y][x].setMap(this);
                              break;
                              //dark
                    case '-': tileptr[y][x].setParameters(tilex,tiley,40,40,-1,this);
                              //tileptr[y][x].setMap(this);
                              break;
                              //background
                    default:  tileptr[y][x].setParameters(tilex,tiley,40,40,1,this);
                              //tileptr[y][x].setMap(this);
                              break;
                }
                x++;
                tilex+=40;  //increment X offset
            }
        }
        y++;
        tiley+=40; //increment Y offset
    }

    reader.close();
    return true;
}

Tile *Map::getTile(int x,int y)
{
    return &tileptr[y][x];
}
int Map::returnCellX(int x){
  
    return ((x-TOFFSETX)/TILESIZE);
}
int Map::returnCellY(int y){

    return ((y-TOFFSETY)/TILESIZE);
}
int Map::getTileX(int a, int b){return tileptr[a][b].getmPosX();}
int Map::getTileY(int a, int b){return tileptr[a][b].getmPosY();}

Tile (*(Map::returngrid)())[COLUMN]{return tileptr;}
/*When a bomb explode, if it has another bomb nearby, this function remove the timer onf the second bomb and make it explode instantly*/
void Map::checkNeighbour(int y,int x){
    
    Bomb* b=tileptr[y][x].getBombP();
    SDL_RemoveTimer(b->getTimer());
    b->explosion2(0);
}
/*This fuction handles the bomb movements across the screen and display the bomb while moving. This could not have been done inside the draw function,because It would cause overlapping drawing
 * between tiles and bombs*/
void Map::checkBombs(SDL_Renderer* g_pRenderer){

    for(int l=0; l<ROW*COLUMN; l++){
        int j=l % (GRIDWIDTH/TILESIZE);
        int i=l / (GRIDWIDTH/TILESIZE);
        if(tileptr[i][j].hasbomb()){
            SDL_Rect *srcBomb=tileptr[i][j].getBombP()->getPlayer()->getBombSrc();
            SDL_Rect  *destBomb=tileptr[i][j].getBombP()->getDstR();
            Bomb *b=tileptr[i][j].getBombP();

            if(b->getKicked()){
                b->bombTravel();
                handleBombMovement(i,j,b);
            }
           
            Texture *bombtexture=b->getPlayer()->getBombT();
            bombtexture->render(g_pRenderer, srcBomb, destBomb, 0.0, NULL, SDL_FLIP_NONE);
            
        }

        if(tileptr[i][j].getExplosion()){
            checkRangeEx(i,j);
	
            tileptr[i][j].incCounter();

        }

    }

}
/*This function checks the boxes that should explode and change into a normal background tile This is done on X axe*/
void Map::checkBoxesX(int y,int start, int end ){ //Tile types: 1=background 2=wall 3=box 4=explosion box

    for(int i=start-1; i<=end+1 && i>=0 && i<COLUMN; i++){
        playersEx=tileptr[y][i].getPlayersExplosion();
        playersExType=tileptr[y][i].getPlayersExplosionType();
        if(tileptr[y][i].getSwap()){
            tileptr[y][i].setType(1);
            tileptr[y][i].setswapTile(false);
            tileptr[y][i].setItemType(calculateItem());
            continue;
        }

        if(playersEx->size()!=0){
            playersEx->clear();
            playersExType->clear();
            //~ playersEx->erase(playersEx->begin());
            //~ playersExType->erase(playersExType->begin());
        }
    }
}
/*This function checks the boxes that should explode and change into a normal background tile This is done on Y axe*/
void Map::checkBoxesY(int x, int start, int end ){ //Tile types: 1=background 2=wall 3=box 4=explosion box

    for(int i=start-1; i<=end+1 && i>=0 && i<ROW; i++){
        playersEx=tileptr[i][x].getPlayersExplosion();
        playersExType=tileptr[i][x].getPlayersExplosionType();
        if(tileptr[i][x].getSwap()){
            tileptr[i][x].setType(1);
            tileptr[i][x].setswapTile(false);
            tileptr[i][x].setItemType(calculateItem());
            continue;
        }
        if(playersEx->size()!=0){
            playersEx->clear();
            playersExType->clear();
            //~ playersEx->erase(playersEx->begin());
            //~ playersExType->erase(playersExType->begin());
        }

    }
}
/*This function is used to calculate the range of explosion for bothe axes*/
void Map::checkRangeEx(int y, int x){

    explosionTexture=tileptr[y][x].getPlayer()->getExplosionT();
    Player *playerExplosion=tileptr[y][x].getPlayer();

    playersEx=tileptr[y][x].getPlayersExplosion();
    //~ if (std::find(playersEx->begin(), playersEx->end(), playerExplosion)!=playersEx->end()){
    //~ return;
    //~ }


    int power=playerExplosion->getPower();


    int start=-1;
    int end=-1;
    for(int i=y; i>=y-power; i--){
        if(i<0)
            break;
        if(tileptr[i][x].getType()==1){
            start=i;
        }else{
            if(tileptr[i][x].getType()==3){
                tileptr[i][x].setswapTile(true);
                tileptr[i][x].setType(4);
                start=i+1;
                break;
            }else{
                start=i+1;
                break;
            }
        }
    }
    for(int i=y; i<=y+power; i++){
        if(i==ROW)
            break;
        if(tileptr[i][x].getType()==1){
            end=i;
        }else{
            if(tileptr[i][x].getType()==3){
                tileptr[i][x].setswapTile(true);
                tileptr[i][x].setType(4);
                end=i-1;
                break;
            }else{
                end=i-1;
                break;
            }
        }
    }
    handleExY(y,x,start,end,playerExplosion);
    if(tileptr[y][x].explosion_finish()){
        checkBoxesY(x,start,end);
	removeFireY(x,start,end);
    }

    start=-1;
    end=-1;
    for(int i=x; i>=x-power; i--){
        if(i<0)
            break;
        if(tileptr[y][i].getType()==1){
            start=i;
        }else{
            if(tileptr[y][i].getType()==3){
                tileptr[y][i].setswapTile(true);
                tileptr[y][i].setType(4);
                start=i+1;
                break;
            }else{
                start=i+1;
                break;
            }
        }
    }
    for(int i=x; i<=x+power; i++){
        if(i==COLUMN)
            break;
        if(tileptr[y][i].getType()==1){
            end=i;
        }else{
            if(tileptr[y][i].getType()==3){
                tileptr[y][i].setswapTile(true);
                tileptr[y][i].setType(4);
                end=i-1;
                break;
            }else{
                end=i-1;
                break;
            }
        }
    }
    handleExX(y,x,start,end,playerExplosion);
    if(tileptr[y][x].explosion_finish()){
        checkBoxesX(y,start,end);
	removeFireX(y,start,end);
        // explosionSrc.y=80; //reset flame
        tileptr[y][x].set_explosion_end(false);
        tileptr[y][x].setExplosion(false);
        playerExplosion->reset_bombNum();
        delete(tileptr[y][x].getBombP());
        tileptr[y][x].setBombP(NULL);
    }
}
/*This function handles the explosion on the Y axe*/
void Map::handleExY(int y,int x, int start, int end, Player* playerExplosion){
    int power=playerExplosion->getPower();

    for(int i=start; i<=end; i++){
	  tileptr[i][x].setFire(true);
        if(tileptr[i][x].hasbomb())
            checkNeighbour(i,x);

        playersEx=tileptr[i][x].getPlayersExplosion();
        playersExType=tileptr[i][x].getPlayersExplosionType();

        if(i==y){
            if(checkExistanceExplosionPlayer(playerExplosion, 3))
                continue;
            tileptr[i][x].addExplosionType(3);
            tileptr[i][x].addPlayerEx(playerExplosion);
        }else if(i==(y+power) ){
            if(checkExistanceExplosionPlayer(playerExplosion, -2))
                continue;
            tileptr[i][x].addExplosionType(-2);
            tileptr[i][x].addPlayerEx(playerExplosion);
        }else if(i==(y-power)){
            if(checkExistanceExplosionPlayer(playerExplosion, 2))
                continue;
            tileptr[i][x].addExplosionType(2);
            tileptr[i][x].addPlayerEx(playerExplosion);
        }else{
            if(checkExistanceExplosionPlayer(playerExplosion, 4))
                continue;
            tileptr[i][x].addExplosionType(4);
            tileptr[i][x].addPlayerEx(playerExplosion);
        }
        checkCollisionExplosionPlayer(tileptr[i][x].getmPosX(), tileptr[i][x].getmPosY());
    }
}
/*This function handles the explosion on the X axe*/
void Map::handleExX(int y,int x, int start, int end, Player* playerExplosion){
    int power=playerExplosion->getPower();

    for(int i=start; i<=end; i++){
	  tileptr[y][i].setFire(true);
        if(tileptr[y][i].hasbomb())
            checkNeighbour(y,i);

        playersEx=tileptr[y][i].getPlayersExplosion();
        playersExType=tileptr[y][i].getPlayersExplosionType();

        if(i==x){
            if(checkExistanceExplosionPlayer(playerExplosion, 3))
                continue;
            tileptr[y][i].addExplosionType(3);
            tileptr[y][i].addPlayerEx(playerExplosion);
        }else if(i==x+power){
            if(checkExistanceExplosionPlayer(playerExplosion, 1))
                continue;
            tileptr[y][i].addExplosionType(1);
            tileptr[y][i].addPlayerEx(playerExplosion);

        }else if(i==x-power){
            if(checkExistanceExplosionPlayer(playerExplosion, -1))
                continue;
            tileptr[y][i].addExplosionType(-1);
            tileptr[y][i].addPlayerEx(playerExplosion);
        }else{
            if(checkExistanceExplosionPlayer(playerExplosion, 5))
                continue;
            tileptr[y][i].addExplosionType(5);
            tileptr[y][i].addPlayerEx(playerExplosion);
        }
        checkCollisionExplosionPlayer(tileptr[y][i].getmPosX(), tileptr[y][i].getmPosY());
    }
}
void Map::checkCollisionExplosionPlayer(int tilex,int tiley){
    int playerX=0;
    int playerY=0;
    for(unsigned int i=0; i<players->size(); i++){
        playerX=players->at(i)->getmPosX()+PLAYEROFFSETX;
        playerY=players->at(i)->getmPosY()+PLAYEROFFSETY;

        if(playerX > tilex && playerX < tilex+40 && playerY>tiley && playerY<tiley+40){
            std::cout<<"Player "<<i <<" is dead "<<std::endl;
            players->at(i)->setDead(true);
        }
    }
}
/*This function calculate the item upon the box explosion. Changing the if condition will affect a more genereous or less generous item quantity
 * It is good not to be too generous in this kind of game :-) */
int Map::calculateItem(){
    //test
    //return 5;

    int rndnumber,result=0;
    rndnumber=rand() % 10000 + 1;
    result=rndnumber % 8;
    if(result>2 && result<6){   // this is to limit the chances of getting an item out of the box. This configuration is still too kind.
        result=rand() % 8 + 1;
        std::cout<<"Number Gen: "<<rndnumber<<" Result "<<result<<std::endl;
        return result;
    }
    return 0;

}
/*This function check is the explision type should be added to the vector which stores the explosion types. 
 * It is not a good way of doing things, I supposed. It used the vector as queue of oxplosion to display. 
 * This was the unic solution I could think of when trying to display everything from the draw map function/*/
bool Map::checkExistanceExplosionPlayer(Player* playerExplosion, int typeN){

    for(unsigned int i=0;i<playersEx->size();i++){
        if(playersEx->at(i) == playerExplosion && playersExType->at(i)==typeN)
            return true;
    }
    return false;
}
/*This is the function manager which calls the function that handles the bomb movement across map*/
void Map:: handleBombMovement(int y, int x, Bomb* b){

    switch(b->getKickedState()){
        case 1: handleBombMovementRight(y,x,b);
                break;
        case -1:handleBombMovementLeft(y,x,b);
                break;
        case 2: handleBombMovementUp(y,x,b);
                break;
        case -2:handleBombMovementDown(y,x,b);
                break;
        default:
                break;
    }


}
/*The following functions handle the movement in all the direction. Obviously if a bomb hit the wall or another player or another bomb, the move is stopped.*/
void Map::handleBombMovementRight(int y, int x,Bomb *b){
    int  bombCellX =returnCellX(b->getPosX()+BOMBW);
    bool hitplayer=false;
    bool move=true;

    for(unsigned int i=0; i<players->size(); i++){
        if(b->getPlayerKicked() != players->at(i)){
            if(bombCollidePlayer(b, players->at(i)))
                hitplayer=true;
        }
    }

    if(bombCellX==x){
        return;
    }
    if(hitplayer)
        move=false;
    else if(tileptr[y][bombCellX].hasbomb())
        move=false;
    else if(tileptr[y][bombCellX].getType()!=1) //1=walkable background
        move=false;


    if(move){
        b->setTile(&tileptr[y][bombCellX]);
        tileptr[y][bombCellX].setBombP(b);
        tileptr[y][bombCellX].setbomb(true);
        tileptr[y][x].setbomb(false);
        tileptr[y][x].setBombP(NULL);
    }else{
        b->travelBack();
        b->setKickedState(0);
        b->setKicked(false);
    }
}
void Map::handleBombMovementLeft(int y, int x,Bomb *b){
    int  bombCellX =returnCellX(b->getPosX());
    bool hitplayer=false;
    bool move=true;
    for(unsigned int i=0; i<players->size(); i++){
        if(b->getPlayerKicked() != players->at(i)){
            if(bombCollidePlayer(b, players->at(i)))
                hitplayer=true;
        }
    }

    if(bombCellX==x){
        return;
    }
    if(hitplayer)
        move=false;
    else if(tileptr[y][bombCellX].hasbomb())
        move=false;
    else if(tileptr[y][bombCellX].getType()!=1) //1=walkable background
        move=false;


    if(move){
        b->setTile(&tileptr[y][bombCellX]);
        tileptr[y][bombCellX].setBombP(b);
        tileptr[y][bombCellX].setbomb(true);
        tileptr[y][x].setbomb(false);
        tileptr[y][x].setBombP(NULL);
    }else{
        b->travelBack();
        b->setKickedState(0);
        b->setKicked(false);
    }
}
void Map::handleBombMovementDown(int y, int x,Bomb *b){
    int  bombCellY =returnCellY(b->getPosY()+BOMBH);
    bool hitplayer=false;
    bool move=true;

    for(unsigned int i=0; i<players->size(); i++){
        if(b->getPlayerKicked() != players->at(i)){
            if(bombCollidePlayer(b, players->at(i)))
                hitplayer=true;
        }
    }

    if(bombCellY==y){
        return;
    }
    if(hitplayer)
        move=false;
    else if(tileptr[bombCellY][x].hasbomb())
        move=false;
    else if(tileptr[bombCellY][x].getType()!=1) //1=walkable background
        move=false;


    if(move){
        b->setTile(&tileptr[bombCellY][x]);
        tileptr[bombCellY][x].setBombP(b);
        tileptr[bombCellY][x].setbomb(true);
        tileptr[y][x].setbomb(false);
        tileptr[y][x].setBombP(NULL);
    }else{
        b->travelBack();
        b->setKickedState(0);
        b->setKicked(false);
    }
}
void Map::handleBombMovementUp(int y, int x,Bomb *b){
    int  bombCellY =returnCellY(b->getPosY());
    bool hitplayer=false;
    bool move=true;
    for(unsigned int i=0; i<players->size(); i++){
        if(b->getPlayerKicked() != players->at(i)){
            if(bombCollidePlayer(b, players->at(i)))
                hitplayer=true;
        }
    }

    if(bombCellY==y){
        return;
    }
    if(hitplayer)
        move=false;
    else if(tileptr[bombCellY][x].hasbomb())
        move=false;
    else if(tileptr[bombCellY][x].getType()!=1) //1=walkable background
        move=false;


    if(move){
        b->setTile(&tileptr[bombCellY][x]);
        tileptr[bombCellY][x].setBombP(b);
        tileptr[bombCellY][x].setbomb(true);
        tileptr[y][x].setbomb(false);
        tileptr[y][x].setBombP(NULL);
    }else{
        b->travelBack();
        b->setKickedState(0);
        b->setKicked(false);
    }
}
/*This function check the collision of a bomb against another player - Only happens when the bomb is travelling*/
bool Map::bombCollidePlayer(Bomb* b, Player *p){
    int playerCellX,playerCellY=0;
    int bombCellX,bombCellY=0;
    bombCellX=returnCellX(b->getPosX()+(BOMBW/2));
    bombCellY=returnCellY(b->getPosY()+(BOMBH/2));
    playerCellY=returnCellY(p->getmPosY()+35);
    playerCellX=returnCellX(p->getmPosX()+20);
    int direction =b->getKickedState();

    if(&tileptr[bombCellX+1][bombCellY] == &tileptr[playerCellX][playerCellY] && direction ==1)
        return true;
    else if (&tileptr[bombCellX-1][bombCellY]==&tileptr[playerCellX][playerCellY] && direction ==-1)
        return true;
    else if(&tileptr[bombCellX][bombCellY+1]==&tileptr[playerCellX][playerCellY] && direction==-2)
        return true;
    else if(&tileptr[bombCellX][bombCellY-1]==&tileptr[playerCellX][playerCellY] && direction == 2)
        return true;
    return false;

}
void Map::removeFireX(int y,int start,int end){
  for(int i=start; i<=end; i++){
    tileptr[y][i].setFire(false);
  }
}
void Map::removeFireY(int x,int start,int end){
  for(int i=start; i<=end; i++){
    tileptr[i][x].setFire(false);
  }
}
