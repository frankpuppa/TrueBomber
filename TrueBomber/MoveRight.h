#ifndef _MOVERIGHT_H
#define _MOVERIGHT_H

#include "Command.h"

class MoveRight: public Command{
    public:
    MoveRight();
    ~MoveRight();
    virtual void execute(Player*);
};
#endif
