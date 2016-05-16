#include "InputHandler.h"
#include "MoveDown.h"
#include "MoveUp.h"
#include "MoveLeft.h"
#include "MoveRight.h"
#include "DropBomb.h"
#include "SetTrap.h"
#include "StopMove.h"
#include <iostream>

InputHandler::InputHandler(){
    move_up = new MoveUp();
    move_down = new MoveDown();
    move_left = new MoveLeft();
    move_right = new MoveRight();
    drop_bomb = new DropBomb();
    set_trap = new SetTrap();
    stop_move= new StopMove();
}
InputHandler::~InputHandler(){
    std::cout<<"Deleting InputHandler"<<std::endl;
    delete move_up;
    delete move_down;
    delete move_left;
    delete move_right;
    delete drop_bomb;
    delete set_trap;
    delete stop_move;
}

bool InputHandler::inputMap(){

    SDL_Event e;
    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 ){
        //User requests quit
        if( e.type == SDL_QUIT ){
            *quit=true;
        }else if( e.type == SDL_KEYDOWN ){
            keydown(e);
            //input_handler.updatekeystates();
        }else if( e.type == SDL_KEYUP ){
            keyup();
        }
    }
    return *quit;
}
void InputHandler::handleInput(){}


void InputHandler::keydown(SDL_Event& e){
    switch( e.key.keysym.sym )
    {
        case SDLK_UP:
            move_up->execute(players.at(0));
            break;
        case SDLK_DOWN:
            move_down->execute(players.at(0));
            break;
        case SDLK_RIGHT:
            move_right->execute(players.at(0));
            break;
        case SDLK_LEFT:
            move_left->execute(players.at(0));
            break;
        case SDLK_SPACE:
            drop_bomb->execute(players.at(0));
            break;
        case SDLK_SLASH:
            set_trap->execute(players.at(0));
            break;
        case SDLK_w:
            move_up->execute(players.at(1));
            break;
        case SDLK_s:
            move_down->execute(players.at(1));
            break;
        case SDLK_d:
            move_right->execute(players.at(1));
            break;
        case SDLK_a:
            move_left->execute(players.at(1));
            break;
        case SDLK_BACKSLASH:
            drop_bomb->execute(players.at(1));
            break;
        case SDLK_z:
            set_trap->execute(players.at(1));
            break;
        case SDLK_ESCAPE:
            *quit=true;
            break;
    }

}
void InputHandler::keyup(){

    //Adjust the velocity
    //~ switch( e.key.keysym.sym )
    //~ {
        //~ case SDLK_UP:
            //~ move_up->executeKeyRelease(players->at(0));
            //~ break;
        //~ case SDLK_DOWN:
            //~ move_down->executeKeyRelease(players->at(0));
            //~ break;
        //~ case SDLK_RIGHT:
            //~ move_right->executeKeyRelease(players->at(0));
            //~ break;
        //~ case SDLK_LEFT:
            //~ move_left->executeKeyRelease(players->at(0));
            //~ break;
	    
        //~ case SDLK_SPACE:
            //~ break;
        //~ case SDLK_w:
            //~ move_up->executeKeyRelease(players->at(1));
            //~ break;
        //~ case SDLK_s:
            //~ move_down->executeKeyRelease(players->at(1));
            //~ break;
        //~ case SDLK_d:
            //~ move_right->executeKeyRelease(players->at(1));
            //~ break;
        //~ case SDLK_a:
            //~ move_left->executeKeyRelease(players->at(1));
            //~ break;
        //~ case SDLK_BACKSLASH:
            //~ break;
        //~ case SDLK_ESCAPE:
            //~ break;
    //~ }
   updatekeystates();
}

void InputHandler::updatekeystates(){
    const Uint8 *state= SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_A])
        move_left->execute(players.at(1));
    else if(state[SDL_SCANCODE_D])
        move_right->execute(players.at(1));
    else if(state[SDL_SCANCODE_W])
        move_up->execute(players.at(1));
    else if(state[SDL_SCANCODE_S])
        move_down->execute(players.at(1));
    else
	stop_move->execute(players.at(1));
	
    if (state[SDL_SCANCODE_LEFT])
        move_left->execute(players.at(0));
    else if(state[SDL_SCANCODE_RIGHT])
        move_right->execute(players.at(0));
    else if(state[SDL_SCANCODE_UP])
        move_up->execute(players.at(0));
    else if(state[SDL_SCANCODE_DOWN])
        move_down->execute(players.at(0));
    else
	stop_move->execute(players.at(0));
}
bool InputHandler::setPlayers(std::vector<Player*> * players, std::vector<int> *playerController){
      
      bool success=false;
	//fill vector with pointers
      for(unsigned int i=0; i<players->size(); i++){
	Player* p;
	this->players.push_back(p);
      }
      //every time playerController->size() should be the same as players->size() as the controller should be assigned to every player.
      //we can check that just for security...
      if(players->size()==playerController->size()){
       for(unsigned int i=0; i<players->size(); i++){
	  this->players.at(playerController->at(i)-1)=players->at(i);  //-1 is used to address the right player to right controller. Since controller configuration starts with 1
	}
	success=true;
      }else{
	std::cout<<"N of controllers and players dont't match!"<<std::endl;
	success=false;
      }
    return success;
}
int InputHandler::handleMenuKeys(bool * quit){
    SDL_Event e;
    int menuKey=0;
    while( SDL_PollEvent( &e ) != 0 )
    {
        //User requests quit
        if( e.type == SDL_QUIT )
        {
            *quit = true;
            return menuKey;
        }
        if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
            //Adjust the velocity
            switch( e.key.keysym.sym )
            {
                case SDLK_UP:
                    menuKey=2;
                    break;
                case SDLK_DOWN:
                    menuKey=-2;
                    break;
                case SDLK_RIGHT:
                    menuKey=1;
                    break;
                case SDLK_LEFT:
                    menuKey=-1;
                    break;
                case SDLK_RETURN:
                    menuKey=3;
                    break;
                case SDLK_ESCAPE:
                    menuKey=-3;
                    break;
                default:
                    break;
            }
        }
        break;
    }
    return menuKey;
}
void InputHandler::setEndGame(bool* q){
    quit=q;
}
