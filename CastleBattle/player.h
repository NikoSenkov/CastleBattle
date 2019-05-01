#ifndef PLAYER_H
#define PLAYER_H

#include<iostream>

class Player
{
public:
    Player();
    Player(int s, int a, int g, int c, int vP);

    int stone_;
    int army_;
    int gold_;
    int curLoc;
    int victoryPoints_;


};

#endif // PLAYER_H
