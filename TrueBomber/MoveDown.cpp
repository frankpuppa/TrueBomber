#include "MoveDown.h"
#include "Player.h"
MoveDown::MoveDown(){}
MoveDown::~MoveDown(){}
void MoveDown::execute(Player * player) { player->move_down(); }
