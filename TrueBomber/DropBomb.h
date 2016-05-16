#ifndef _DROPBOMB_H
#define _DROPBOMB_H

#include "Command.h"

class DropBomb : public Command{
    public:
    DropBomb();
	~DropBomb();
    virtual void execute(Player*);
};
#endif
