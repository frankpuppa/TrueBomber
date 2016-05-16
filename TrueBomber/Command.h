#ifndef _COMMAND_H
#define _COMMAND_H

/*This is an abstract class, or rather an interface used by the Command design patter
 * The classes implementing it, basically perform action in the game*/

class Player;
class Command{
    public:
        virtual ~Command(){}
        virtual void execute(Player *)=0; // Pure virtual method =0 same as an interface. The method must be overidden                           //virtual void execute(){}; --> This is how it would work for an abstract class.
};

#endif
