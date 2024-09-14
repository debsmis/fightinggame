// Game.h

#ifndef GAME_INCLUDED
#pragma once
#define GAME_INCLUDED

#include "Temple.h"
#include "Monster.h"

// You may add data members and other member functions to this class.

class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game();
    void play();
    void display();
    void move(int xDir, int yDir);
    void removeItemAtPlayerPosition(int x, int y);
    void generateMonsters(int level);
    

    void initializeLevel(int level);


private:
    vector<Temple> temples;

    int goblinSmellDistance;

    Player* player;
    int currLevel;

    vector<GameObject*> globalInventory{ new shortSword() };  // player inventory

};

#endif // GAME_INCLUDED

