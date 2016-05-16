#include "MoveRight.h"
#include "Player.h"
MoveRight::MoveRight(){}
MoveRight::~MoveRight(){}
void MoveRight::execute(Player * player) { player->move_right(); }

