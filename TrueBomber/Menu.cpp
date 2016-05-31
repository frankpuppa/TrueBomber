#include "Menu.h"
#include "Map.h"
#include "Player.h"
#include "TextureTTF.h"
#include "Sound.h"

#include <sstream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

/*Initialization of static values*/
std::string const Menu::menusText[3]= {" New Game ","   Audio   ", "  Exit   "};
std::string const Menu::menusText2[2]= {" 1 Player ", " 2 Players "};
std::string const Menu::menuTitles[6]={" Welcome ", " Audio Config ", " Choose N Players ", " Choose Map ", " Controllers View "," Choose Player's Features "};
std::string const Menu::audioText[2]= {" Audio ON ", " Audio OFF "};
/*Menu Contructor, initilize values used in the menu*/
Menu::Menu(){
    g_pWindow=NULL;
    g_pRenderer=NULL;
    menu1=true;
    menu2=false;
    menu3=false;
    menu4=false;
    menu5=false;
    menu6=false;
    player_number=0;
    pointerRect=NULL;
    Expsrc=NULL;
    Expdest=NULL;
    Plyrsrc=NULL;
    Plyrdest=NULL;
    Bombsrc=NULL;
    Bombdest=NULL;
    ControllerDestR=NULL;
    ControllerSourceR=NULL;
    indexExplosion=NULL;
    indexPlayer=NULL;
    indexBomb=NULL;
    unavailable=false;
   
    menuEntryPtr=NULL;
    g_pFont=NULL;
    playersTTF=NULL;
    playerControllerConfig.push_back(1);
    playerControllerConfig.push_back(2);
}

Menu::~Menu(){
    std::cout<<"Cleaning Menu Deconstructor"<<std::endl;
    free();
}
/*This function initialize all the required textures and features to correctly display the menu, If any of these fails, Menu will not be available.*/
bool Menu::init(Texture* bombs,Texture *backgr, TTF_Font * font, InputHandler* handler, SDL_Renderer* renderer, SDL_Window* window){
    
      
      g_pRenderer=renderer;
      g_pWindow=window;
    
      if(!Players.loadTexture("pics/selectPl.png", g_pRenderer)){
	  std::cout<<"Players cannot be loaded\n";
	  return false;
      }
      if(!Explosions.loadTexture("pics/selectEx.png", g_pRenderer)){
	  std::cout<<"Explosions cannot be loaded\n";
	  return false;
      }
      
      if(!ControllersT.loadTexture("pics/inputkey.png",g_pRenderer)){
	 std::cout<<"Keyboard pic cannot be loaded\n";
	  return false;
      }
      
      if(!mapfilesHandler.loadMapsfromDir(&maps)){
	  std::cout<<"Maps cannot be loaded from directory maps/ \n";
	  return false;
      }
      g_pFont=font;
      Bombs=bombs;
      background=backgr;
      input_handler=handler;
      
      
    return true;
}

void Menu::free(){
    std::cout << "cleaning Menu Class\n";
    maps.clear();
    freeDynamicMemory();
}
/*Dinamic function which setup each menu accordigly to request*/
void Menu::setupMenu(){

    SDL_Color menuTColor={102,20,0,100};
    SDL_Color menuEColor={255,0,0,100};
    if(menuEntryVect.size()!=0)
        menuEntryVect.clear();

    if(menu1){
        menuEntryVect.assign(menusText, menusText + sizeof(menusText)/sizeof(menusText[0]));
        menuEntryPtr=&menuEntryVect.at(0);
        menuTitle.loadFromRenderedText(menuTitles[0],menuTColor,g_pRenderer, g_pFont);
    }else if(menu2){
        menuEntryVect.assign(menusText2, menusText2 + sizeof(menusText2)/sizeof(menusText2[0]));
        menuEntryPtr=&menuEntryVect.at(0);
        menuTitle.loadFromRenderedText(menuTitles[2], menuTColor, g_pRenderer, g_pFont);
    }else if(menu3){
        menuEntryPtr=&maps.at(0);
        menuTitle.loadFromRenderedText(menuTitles[3],menuTColor,g_pRenderer,g_pFont);
    }else if(menu4){
        menuTitle.loadFromRenderedText(menuTitles[5],menuTColor, g_pRenderer,g_pFont);
        initTextures();
        return;
    }else if(menu5){
      menuEntryVect.assign(audioText, audioText + sizeof(audioText)/sizeof(audioText[0]));
      menuTitle.loadFromRenderedText(menuTitles[1],menuTColor, g_pRenderer,g_pFont);
      menuEntryPtr=&menuEntryVect.at(0);
    }else if(menu6){
      menuTitle.loadFromRenderedText(menuTitles[4],menuTColor, g_pRenderer,g_pFont);
      setupPlayersTTF();
      setupControllers();
    }

    menuEntry.loadFromRenderedText(*menuEntryPtr,menuEColor,g_pRenderer,g_pFont);
}
/*This function only display the menu. Upon user choice is display the correct menu entry.*/
void Menu::displayMenu(bool *q,bool* s){

    int yoffset=0;
    TextureTTF temporaryTexture;
    this->quit=q;
    this->state_ok=s;
    freeDynamicMemory();
    if(!background->loadTexture("pics/walper.jpg", g_pRenderer)){
        std::cout<<"Background cannot be loaded\n";
        return;
    }

    Sound::loadMusic("audio/musicmenu.wav");
    setupMenu();

    SDL_Color textColor={102,20,0,100};
    temporaryTexture.loadFromRenderedText("Not yet available",textColor,g_pRenderer,g_pFont);

    Sound::soundHandler(8);
    Sound::setMusicVolume(80);
    while( !*state_ok && !*quit )
    {
        handleMenus();

        clearScreenRenderBG();
        if(menu4){
            renderCharacterChoice();
            if(unavailable){
                temporaryTexture.render(g_pRenderer, (SCREEN_WIDTH - temporaryTexture.getWidth())/2, 450);
            }
        }else if(menu6){
	  displayControllers();
	}else{
		  displayRectMenu();
		  yoffset=-40;
		  menuEntry.render(g_pRenderer, (( SCREEN_WIDTH - menuEntry.getWidth() ) / 2),( ( SCREEN_HEIGHT - menuEntry.getHeight() ) / 2 ) +yoffset);
        }
	menuTitle.render(g_pRenderer,((SCREEN_WIDTH-menuTitle.getWidth())/2),160);
        SDL_RenderPresent(g_pRenderer);
    }
      Sound::stopMusic();
      Sound::freeMusic();
}
/*Function used to setup the game map*/
void Menu::setupMap(Map *map,std::vector<Player*> *players){
    if(!map->init(g_pRenderer, "pics/maptiles.png", players, *menuEntryPtr)){
        std::cout<<"Map texture error"<<std::endl;
    }

}
/*Function used to setup the players characteristics*/
void Menu::setupPlayers(Map* map,std::vector<Player*> *players){
    std::string playerfile;
    std::string playerflame;
    
    for(int i=0; i<player_number; i++){

        playerfile="pics/iconp"+ to_string(indexPlayer[i]) + ".png";
        playerflame="pics/flame"+ to_string(indexExplosion[i]) + ".png";

        players->push_back(new Player(playerfile,map));

        if(! players->at(i)->init(g_pRenderer,playerfile, playerflame, Bombs, Bombsrc[i].x, Bombsrc[i].y)){
            std::cout<<"Player texture cannot be initialized\n";
        }
    }
}
/*Function that handles the menu decision based on the arrow, Up,Down,Left,Right +Escape and Enter key*/
void Menu::handleMenus(){
    //Handle events on queue
    switch(input_handler->handleMenuKeys(quit)){
        case -1:
            handleLEFT();
            break;
        case 1:
            handleRIGHT();
            break;
        case -2:
            handleDOWN();
            break;
        case 2:
            handleUP();
            break;
        case -3:
            handleESCAPE();
            break;
        case 3:
            handleRETURN();
            break;
        default:
            break;
    }
}
/*The following function handle the changes upon menu selected and key pressed*/
void Menu::handleUP(){
    SDL_Color textColor = {255,0,0,100};
    std::vector<std::string> *temp;
    if(menu6){
      for (int i=0; i<player_number; i++){
	if(pointerRect == &ControllerDestR[i]){
	    if(ControllerSourceR[i].y==0){
		ControllerSourceR[i].y=60;
		ControllerSourceR[i+1].y=0;
		playerControllerConfig.at(0)=2;
		playerControllerConfig.at(1)=1;
	    }else{
		ControllerSourceR[i].y=0;
		ControllerSourceR[i+1].y=60;
		playerControllerConfig.at(0)=1;
		playerControllerConfig.at(1)=2;
	    }
	}
      }
      std::cout<<"Player1: "<<playerControllerConfig.at(0) <<" Player 2: "<<playerControllerConfig.at(1)<<std::endl;
      return;
    }
      
    if(menu3)
      temp=&maps;
    else
      temp=&menuEntryVect;
    if(!menu4){
        ++menuEntryPtr;
        if(menuEntryPtr>&temp->at(temp->size()-1))
            menuEntryPtr=&temp->at(0);
        menuEntry.loadFromRenderedText(*menuEntryPtr,textColor,g_pRenderer,g_pFont);
    }else{
        for(int i=0; i<player_number; i++){
            if(pointerRect==&Plyrdest[i]){
                Plyrsrc[i].y-=PLAYERH;
                indexPlayer[i]--;
                if(Plyrsrc[i].y < 0){
                    Plyrsrc[i].y=(PLCHOICE-1)*PLAYERH;
                    indexPlayer[i]=PLCHOICE-1;
                }
            }else if(pointerRect==&Expdest[i]){
                Expsrc[i].y-=EXPLOSIONH;
                indexExplosion[i]--;
                if(Expsrc[i].y < 0){
                    Expsrc[i].y=(EXCHOICE-1)*EXPLOSIONH;
                    indexExplosion[i]=EXCHOICE-1;
                }
            }else if(pointerRect==&Bombdest[i]){
                Bombsrc[i].y-=BOMBH;
                indexBomb[i]--;
                if(Bombsrc[i].y < 0){
                    if(Bombsrc[i].x > 0){
                        Bombsrc[i].y=BOMBH*((BMCHOICE/2)-1);
                        Bombsrc[i].x=0;
                        indexBomb[i]--;
                    }else{
                        Bombsrc[i].y=BOMBH*((BMCHOICE/2)-1);
                        Bombsrc[i].x=BOMBW *4;
                        indexBomb[i]=BMCHOICE-1;
                    }
                }
            }
        }
    }
}
/*The following function handle the changes upon menu selected and key pressed*/
void Menu::handleDOWN(){
    SDL_Color textColor = {255,0,0,100};
    std::vector<std::string> *temp;
    if(menu6){
      for (int i=0; i<player_number; i++){
	if(pointerRect == &ControllerDestR[i]){
	    if(ControllerSourceR[i].y==0){
		ControllerSourceR[i].y=60;
		ControllerSourceR[i+1].y=0;
		playerControllerConfig.at(0)=2;
		playerControllerConfig.at(1)=1;
	    }else{
		ControllerSourceR[i].y=0;
		ControllerSourceR[i+1].y=60;
		playerControllerConfig.at(0)=1;
		playerControllerConfig.at(1)=2;
	    }
	}
      }
      std::cout<<"Player1: "<<playerControllerConfig.at(0) <<" Player 2: "<<playerControllerConfig.at(1)<<std::endl;
      return;
    }
    if(menu3)
      temp=&maps;
    else
      temp=&menuEntryVect;
    if(!menu4){
        --menuEntryPtr;
        if(menuEntryPtr<&temp->at(0))
            menuEntryPtr=&temp->at(temp->size()-1);
        menuEntry.loadFromRenderedText(*menuEntryPtr,textColor,g_pRenderer,g_pFont);
    }else{
        for(int i=0; i<player_number; i++){
            if(pointerRect==&Plyrdest[i]){
                Plyrsrc[i].y+=PLAYERH;
                indexPlayer[i]++;
                if(Plyrsrc[i].y > (PLCHOICE-1)*PLAYERH){
                    Plyrsrc[i].y=0;
                    indexPlayer[i]=0;
                }
            }else if(pointerRect==&Expdest[i]){
                Expsrc[i].y+=EXPLOSIONH;
                indexExplosion[i]++;
                if(Expsrc[i].y > (EXCHOICE-1)*EXPLOSIONH){
                    Expsrc[i].y=0;
                    indexExplosion[i]=0;
                }
            }else if(pointerRect==&Bombdest[i]){
                Bombsrc[i].y+=BOMBH;
                indexBomb[i]++;
                if(Bombsrc[i].y > ((BMCHOICE/2)-1)*BOMBH){
                    if(Bombsrc[i].x > 0){
                        Bombsrc[i].y=0;
                        Bombsrc[i].x=0;
                        indexBomb[i]=0;
                    }else{
                        Bombsrc[i].y=0;
                        Bombsrc[i].x=BOMBW*(BMCHOICE/2);
                        indexBomb[i]++;
                    }
                }
            }
        }
    }
}
/*The following function handle the changes upon menu selected and key pressed*/
void Menu::handleRIGHT(){
  if(menu4){
    for(int i=0; i<player_number; i++){
        if(pointerRect==&Plyrdest[i]){
            pointerRect=&Expdest[i];
            break;
        }else if(pointerRect==&Bombdest[i]){
            if(i<player_number-1){
                pointerRect=&Plyrdest[i+1];
            }else{
                pointerRect=&Plyrdest[0];
            }
            break;
        }else if(pointerRect==&Expdest[i]){
            pointerRect=&Bombdest[i];
            break;
        }
    }
  }else if(menu6){
    //~ if(pointerRect == &ControllerDestR[0])
	//~ pointerRect=&ControllerDestR[player_number-1];
    //~ else if(pointerRect == &ControllerDestR[player_number-1])
	//~ pointerRect=&ControllerDestR[0];
  }
}
/*The following function handle the changes upon menu selected and key pressed*/
void Menu::handleLEFT(){
  if(menu4){
    for(int i=0; i<player_number; i++){
        if(pointerRect==&Plyrdest[i]){
            if(pointerRect==Plyrdest)
                pointerRect=&Bombdest[player_number-1];
            else
                pointerRect=&Bombdest[i-1];
            break;
        }else if(pointerRect==&Bombdest[i]){
            pointerRect=&Expdest[i];
            break;
        }else if(pointerRect==&Expdest[i]){
            pointerRect=&Plyrdest[i];
            break;
        }
    }
  }else if(menu6){
    //~ if(pointerRect == &ControllerDestR[0])
	//~ pointerRect=&ControllerDestR[player_number-1];
    //~ else if(pointerRect == &ControllerDestR[player_number-1])
	//~ pointerRect=&ControllerDestR[0];
  }
}
/*The following function handle the changes upon menu selected and key pressed*/
void Menu::handleRETURN(){
    Sound::soundHandler(1);
    SDL_Delay(400); //add some delay to make it neater..
    if(!menu4){
        menuSelect();
        setupMenu();
        //set map name for the game
    }else{
        if(player_number==1){
            unavailable=true;
            return;
        }
        *state_ok=true;

        std::cout<<"indexPlayer: "<<indexPlayer[0]<<" indexExplosion: "<<indexExplosion[0]<<" Mapname: "<<*menuEntryPtr<<std::endl;
    }
}
/*The following function handle the changes upon menu selected and key pressed
 * This fuction move back to the second main menu.*/
void Menu::handleESCAPE(){
    if(menu1){
        return;
    }else{
        menuEscape();
        setupMenu();
    }
}
/*This function changes the current menu to the next correct menu*/
void Menu::menuSelect(){
    if(menu1){
        //if exit, quit
        if(menuEntryPtr==&menuEntryVect.at(2)){
            *quit=true;
            return;
        }else if(menuEntryPtr==&menuEntryVect.at(1)){ //Audio
	  menu5=true;
	}else if(menuEntryPtr==&menuEntryVect.at(0)){ //Player Select
	  menu2=true;
	}
        menu1=false;
	
    }else if(menu2){
        menu2=false;
        menu3=true;
        //set the total number of players
        if( menuEntryPtr==&menuEntryVect.at(0)){
            player_number=1;
        }else if(menuEntryPtr==&menuEntryVect.at(1)){
            player_number=2;
        }
    }else if(menu3){
        menu3=false;
        menu6=true;
    }else if(menu5){
	handleMusic();
	menu5=false;
	menu1=true;
    }else if(menu6){
      menu4=true;
      menu6=false;
    }
}
/*This function changes the current menu to the previous menu, handling the menu states*/
void Menu::menuEscape(){
    if(menu2){
        menu1=true;
        menu2=false;
    }else if(menu3){
        menu2=true;
        menu3=false;
    }else if(menu4){
        menu2=true;
        menu4=false;
        unavailable=false;
        player_number=0;
    }else if(menu5){
	menu1=true;
	menu5=false;
    }else if(menu6){
      menu2=true;
      menu6=false;
    }
}
/*This function renders on screen the character the player can choose*/
void Menu::renderCharacterChoice(){
      
    for(int i=0; i<player_number; i++){
	playersTTF[i].render(g_pRenderer, Plyrdest[i].x - (playersTTF[i].getWidth()+(playersTTF[i].getWidth()/2))  , Plyrdest[i].y + (playersTTF[i].getHeight()/2) );
        Players.render(g_pRenderer,&Plyrsrc[i],&Plyrdest[i]);
        Explosions.render(g_pRenderer,&Expsrc[i],&Expdest[i]);
        Bombs->render(g_pRenderer, &Bombsrc[i], &Bombdest[i]);
    }

    SDL_SetRenderDrawColor(g_pRenderer, 255,0,0,255);
    SDL_RenderDrawRect(g_pRenderer, pointerRect);
}
/*This function initilizes the dynamic allocated textures from which the player can choose the characters and features - menu4  */
void Menu::initTextures(){
    
    freeDynamicMemory();

    Plyrsrc= new SDL_Rect[player_number](); //() Default initialization, set to 0 every field
    Expsrc= new SDL_Rect[player_number]();
    Plyrdest= new SDL_Rect[player_number]();
    Expdest= new SDL_Rect[player_number]();
    Bombsrc=new SDL_Rect[player_number]();
    Bombdest=new SDL_Rect[player_number]();

    indexExplosion=new int[player_number]();
    indexPlayer=new int[player_number]();
    indexBomb=new int[player_number]();

    pointerRect=Plyrdest;

    int y=SCREEN_HEIGHT/3;
    int x=SCREEN_WIDTH/4;
    
    setupPlayersTTF();

    for(int i=0; i<player_number; i++){
        Plyrsrc[i]=Players.getTextureSize();
        Expsrc[i]=Explosions.getTextureSize();
        Bombsrc[i]=Bombs->getTextureSize();

        Plyrsrc[i].x=Plyrsrc[i].y=0;
        Plyrsrc[i].w=40;
        Plyrsrc[i].h=60;
        Plyrdest[i].x=220;
        Plyrdest[i].y=y;
        Plyrdest[i].w=40;
        Plyrdest[i].h=60;

        Expsrc[i].x=Expsrc[i].y=0;
        Expsrc[i].w=120;
        Expsrc[i].h=40;
        Expdest[i].x=300;
        Expdest[i].y=y +10; //10 is a bit of offset
        Expdest[i].w=120;
        Expdest[i].h=40;

        Bombsrc[i].x=Bombsrc[i].y=0;
        Bombsrc[i].w=Bombsrc[i].h=40;
        Bombdest[i].x=450;
        Bombdest[i].y=y+10;
        Bombdest[i].w=Bombdest[i].h=40;

        x+=70;
        y+=100;

    }
}
/*This function free the dynaimic allocated textures, allocated in the function above..*/
void Menu::freeDynamicMemory(){
    if(indexExplosion!=NULL || indexPlayer!=NULL || indexBomb!=NULL){
        std::cout << "cleaning indexExplosion\n";
        delete[] indexExplosion;
        indexExplosion=NULL;
        std::cout << "cleaning indexPlayer\n";
        delete[] indexPlayer;
        indexPlayer=NULL;
        std::cout << "cleaning indexBomb\n";
        delete[] indexBomb;
        indexBomb=NULL;
    }
    if(Plyrsrc!=NULL || Expsrc !=NULL || Bombsrc!=NULL){
        std::cout << "cleaning Plysrc\n";
        delete[] Plyrsrc;
        Plyrsrc=NULL;
        std::cout << "cleaning Expsrc\n";
        delete[] Expsrc;
        Expsrc=NULL;
        std::cout << "cleaning Bombsrc\n";
        delete[] Bombsrc;
        Bombsrc=NULL;
    }
    if(Plyrdest!=NULL || Expdest!=NULL || Bombdest!=NULL ){
        std::cout << "cleaning Plyrdest\n";
        delete[] Plyrdest;
        Plyrdest=NULL;
        std::cout << "cleaning Expdest\n";
        delete[] Expdest;
        Expdest=NULL;
        std::cout << "cleaning Bombdest\n";
        delete[] Bombdest;
        Bombdest=NULL;
    }
    freeControllers();
    freePlayersTTF();
}

/*Helper function to convert a Template value to a string*/
template<typename T>std::string Menu::to_string(T value){
    std::ostringstream convert;
    convert << value;
    return convert.str();
}
/*Function used to reset the menus to the first menu - Used when in game to go back to the game menu */
void Menu::resetMenus(){
    menu6=false;
    menu5=false;
    menu4=false;
    menu3=false;
    menu2=false;
    menu1=true;
}
/*Simple function that clear whatever is rendered on screen and draw background*/
void Menu::clearScreenRenderBG(){
    SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear( g_pRenderer );
    background->render(g_pRenderer,NULL,NULL);
}
/*Function used to dinamic allocate the players labels in menu4  */
void Menu::setupPlayersTTF(){
  SDL_Color textColor={0,0,0,100};
  
  freePlayersTTF();
      
  if(playersTTF==NULL)
    playersTTF= new TextureTTF[player_number]();
  
  for(int i=0; i<player_number; i++){
      playersTTF[i].loadFromRenderedText("Player " + to_string(i+1),textColor,g_pRenderer,g_pFont);
    }
}
/* Function used to free the dinamic allocated TTF label used in menu4*/
void Menu::freePlayersTTF(){
  if(playersTTF != NULL){
      std::cout << "cleaning players TTF\n";
        delete[] (playersTTF);
	playersTTF=NULL;
    }
}
/*This function handle the sounds state of the game. The player can choose to switch of sound effects*/	
void Menu::handleMusic(){
    if((Sound::getAudioStatus() && menuEntryPtr==&menuEntryVect.at(0)) || (!Sound::getAudioStatus() && menuEntryPtr==&menuEntryVect.at(1) )){
      return;
    }else{
      if(Sound::getAudioStatus() && menuEntryPtr==&menuEntryVect.at(1)){
	Sound::setAudioOn(false);
	Sound::stopMusic();
      }else{
	Sound::setAudioOn(true);
	Sound::soundHandler(8);
      }
    }
}	
/*This function display the background rectangle in the menu. Use alpha channel to add transparency*/
void Menu::displayRectMenu(){
  SDL_Rect t;
    t.w=280;
    t.h=300;
    t.x=(SCREEN_WIDTH/2)-(t.w/2);
    t.y=(SCREEN_HEIGHT/2)-(t.h/2);
    
    SDL_SetRenderDrawBlendMode(g_pRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(g_pRenderer, 100, 90, 19, 30); //color of the rectangle with alpha transparency
    SDL_RenderFillRect(g_pRenderer, &t);
}
/*This function displays the controllers to choose from in the menu*/
void Menu::displayControllers(){
  int y=SCREEN_WIDTH/2;
  y-=150;
    
  for(int i=0; i<player_number; i++){
    playersTTF[i].render(g_pRenderer, (SCREEN_WIDTH/4)-(playersTTF[i].getWidth()/2)  , y-(playersTTF[i].getHeight()/2) );
    ControllerDestR[i].y=y-(ControllerDestR[i].h/2);
    ControllersT.render(g_pRenderer,&ControllerSourceR[i], &ControllerDestR[i]);
    y+=100;
  }
  
    SDL_SetRenderDrawColor(g_pRenderer, 255,0,0,255);
    SDL_RenderDrawRect(g_pRenderer, pointerRect);
  
}
/*This function set up and allocate resources  for the controllers  used in the menu*/
void Menu::setupControllers(){
  
  freeControllers();
  
  if(ControllerDestR==NULL )
      ControllerDestR =new SDL_Rect[player_number];
  
  if(ControllerSourceR==NULL)
      ControllerSourceR=new SDL_Rect[player_number];
    
    int y=0;
    for(int i=0; i<player_number; i++){
      ControllerSourceR[i]=ControllersT.getTextureSize();
    
      ControllerSourceR[i].x=0;
      ControllerSourceR[i].y=y;
      ControllerSourceR[i].h=60;
      
      ControllerDestR[i].x=SCREEN_WIDTH/4 +120;
      ControllerDestR[i].w=ControllerSourceR[i].w;
      ControllerDestR[i].h=ControllerSourceR[i].h;
      y+=60;
    }
    pointerRect=&ControllerDestR[0];
    
    /*reset controllers to default*/
    playerControllerConfig.at(0)=1;
    playerControllerConfig.at(1)=2;
}
/*This function frees the dynamic allocated resources used for displaying the controllers in menu 6*/
void Menu::freeControllers(){
  
  if(ControllerDestR != NULL || ControllerSourceR !=NULL){
    delete[](ControllerDestR);
    ControllerDestR=NULL;
    delete[](ControllerSourceR);
    ControllerSourceR=NULL;
  }
}
std::vector<int>* Menu::getPlayersControllerConfig(){
  return &playerControllerConfig;
}
/*At the end of the game this function set up next map - Could have just been in the Game class, but it easier this way, since all the data
 * necessary is in this class!*/
void Menu::setupNextMap(){
  int rndnumber=rand() % maps.size();
  menuEntryPtr=&maps.at(rndnumber);
}
