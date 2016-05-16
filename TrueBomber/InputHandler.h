#ifndef _INPUTHANDLER_H
#define _INPUTHANDLER_H


class Command;
class Player;
#include <SDL2/SDL.h>
#include <vector>



class InputHandler{

    public:
        InputHandler();
        ~InputHandler();
        void handleEvents(SDL_Event&);
        void handleInput();
        void keydown(SDL_Event&);
        void keyup();
        bool inputMap();
        void updatekeystates();
        bool setPlayers(std::vector<Player*> *, std::vector<int> *);
        int handleMenuKeys(bool*);
        void setEndGame(bool*);
    private:
        //Player *player_all;
        Command *move_up;
        Command *move_down;
        Command *move_left;
        Command *move_right;
        Command *drop_bomb;
        Command *set_trap;
	Command *stop_move;
        std::vector<Player*> players;
        bool *quit;


};

#endif
