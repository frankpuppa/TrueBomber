#ifndef _MOVEUP_H
#define _MOVEUP_H

#include "Command.h"

class MoveUp : public Command{
    public:
    MoveUp();
    ~MoveUp();
    virtual void execute(Player*);
};
#endif
