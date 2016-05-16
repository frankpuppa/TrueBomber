#include "Game.h"
#include "Menu.h"
#include <iostream>

int main(){
    Game game;
    if(game.init()){
	game.run();
    }
    return 0;
}
