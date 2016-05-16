#include "StopMove.h"
#include "Player.h"
StopMove::StopMove(){}
StopMove::~StopMove(){}
void StopMove::execute(Player * player) { player->stop_move(); }

