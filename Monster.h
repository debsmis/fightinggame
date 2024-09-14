#pragma once

#ifndef MONSTER_H
#define MONSTER_H

#include "Actor.h"
#include "Weapon.h"

class Player;

class Monster : public Actor {
public:
    Monster(const std::string& type, int row, int col, int hit, int armor, int strength, int dexterity, Weapon* weapon);
    virtual ~Monster() {}
    string attack(Player* player);

private:
    Weapon* currentWeapon;
};

class Bogeyman : public Monster {
public:
    Bogeyman(int row, int col);
    virtual ~Bogeyman() {}
};

class Snakewoman : public Monster {
public:
    Snakewoman(int row, int col);
    virtual ~Snakewoman() {}
};

class Dragon : public Monster {
public:
    Dragon(int row, int col);
    virtual ~Dragon() {}
};

class Goblin : public Monster {
public:
    Goblin(int row, int col);
    virtual ~Goblin() {}
};

#endif
