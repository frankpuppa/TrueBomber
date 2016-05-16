#ifndef _MOVELEFT_H
#define _MOVELEFT_H

#include "Command.h"

class MoveLeft : public Command{
    public:
    MoveLeft();
    ~MoveLeft();
    virtual void execute(Player*);
};
#endif
