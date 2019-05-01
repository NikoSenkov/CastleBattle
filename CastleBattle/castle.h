#ifndef CASTLE_H
#define CASTLE_H

#include "tile.h"

class Castle : public Tile
{
    Q_OBJECT
public:
    Castle();
    Castle(int cA);
    virtual ~Castle() { };

    void doBattle(Player *p1, Player *p2);

    int castleAlias;

};

#endif // CASTLE_H
