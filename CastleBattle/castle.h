#ifndef CASTLE_H
#define CASTLE_H

#include "tile.h"

class Castle : public Tile
{
    Q_OBJECT
public:
    Castle();
    virtual ~Castle() { };


};

#endif // CASTLE_H
