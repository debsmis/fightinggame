#pragma once

#ifndef WEAPON_H
#define WEAPON_H

#include "GameObject.h"


class Weapon : public GameObject {
public:
	Weapon(string name, int dexterity, int damage) : GameObject(name), dexterityBonus(dexterity), damageAmount(damage) {}
	void performAction() {}
	int getDexterityBonus() {
		return dexterityBonus;
	}
	int getDamageAmount() {
		return damageAmount;
	}
	
	virtual ~Weapon() {}

private:
	int dexterityBonus;
	int damageAmount;
};

#endif

class Mace : public Weapon {
public:
	Mace() : Weapon("Mace", 0, 2) {}
};

class shortSword : public Weapon {
public:
	shortSword() : Weapon("Short Sword", 0, 2) {}
};


class longSword : public Weapon {
public:
	longSword() : Weapon("Long Sword", 2, 4) {}
};

class magicAxe : public Weapon {
public:
	magicAxe() : Weapon("Magic Axe", 5, 5) {}
};

class magicFangs : public Weapon {
public:
	magicFangs() : Weapon("Magic Fangs", 3, 2) {}
	// In addition to the regular hit point damage magic fangs of sleep do when the attacker hits the defender,
	//  there is a 1 in 5 chance that the magic fangs of sleep will put the defender to sleep. 
	// The number of moves the sleep will last (the sleep time) is a random integer from 2 to 6, call it X. 
	// If the defender is already asleep, with current sleep time Y, then the defender's sleep time becomes the 
	// maximum of X and Y (and not, say, Y+X).
};
