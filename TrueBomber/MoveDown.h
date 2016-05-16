#ifndef _MOVEDOWN_H
#define _MOVEDOWN_H

#include "Command.h"

class MoveDown : public Command{
    public:
    MoveDown();
    ~MoveDown();
    virtual void execute(Player*);
};
#endif
