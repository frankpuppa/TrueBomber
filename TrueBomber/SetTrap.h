#ifndef _SetTrap_H
#define _SetTrap_H

#include "Command.h"

class SetTrap : public Command{
    public:
    SetTrap();
    ~SetTrap();
    virtual void execute(Player*);
};
#endif
