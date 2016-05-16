#include "MoveLeft.h"
#include "Player.h"
MoveLeft::MoveLeft(){}
MoveLeft::~MoveLeft(){}
void MoveLeft::execute(Player * player) { player->move_left(); }


