#include "DropBomb.h"
#include "Player.h"
DropBomb::DropBomb(){}
DropBomb::~DropBomb(){}
void DropBomb::execute(Player * player) { player->drop_bomb(); }
