#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"
#include "Weapon.h"
#include "Scroll.h"
#include "utilities.h"
#include "Monster.h"

class Player : public Actor {
public:
    Player(int row, int col);

    virtual ~Player(); // because new object added to vector

    
    // quit stuff
    void cheat();
    void result(); //check return type

    void pickUpItem(Scroll* item);
    void pickUpItem(Weapon* item);
    void pickUpItem(GameObject* item);
    void printInventory();

    void readScroll(char c);
    void applyScrollEffect(const string& scrollType);

    
    void wieldWeapon(char c);

    bool attack(Monster* monster);

    void actionMessage(const std::string& message);
    string readActionMessage() const;

    vector<GameObject*> getInventory() const;
    void addToInventory(const std::vector<GameObject*>& globalInventory);
    void clearInventory();


private:

    vector<GameObject*> playerObj{};
    Weapon* currentWeapon = nullptr;
    string actionMsg;
};


#endif