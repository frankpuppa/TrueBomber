#include "SetTrap.h"
#include "Player.h"
SetTrap::SetTrap(){}
SetTrap::~SetTrap(){}
void SetTrap::execute(Player * player) { player->installTrap(); }
