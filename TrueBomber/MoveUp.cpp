#include "MoveUp.h"
#include "Player.h"
MoveUp::MoveUp(){}
MoveUp::~MoveUp(){}
void MoveUp::execute(Player * player) { player->move_up(); }



