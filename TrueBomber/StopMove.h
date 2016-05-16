#ifndef _STOPMOVE_H
#define _STOPMOVE_H

#include "Command.h"

class StopMove : public Command{
    public:
    StopMove();
    ~StopMove();
    virtual void execute(Player*);
};
#endif
