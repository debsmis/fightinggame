#pragma once

#ifndef ACTOR_H
#define ACTOR_H

#include "utilities.h"
#include <string>
#include <algorithm>
using namespace std;

class Actor {

public:
	Actor(int row, int col, int hit, int armor, int strength, int dexterity, int maxHit);

	virtual ~Actor();

	void setXPos(int x);
	void setYPos(int y);

	int getXPos() const;
	int getYPos() const;

	void setHitPoints(int hp);
	int getHitPoints() const;

	void changeMaxHit(int h);
	int getMaxHit() const;


	void setArmorPoints(int armor);
	int getArmorPoints() const;

	void setStrengthPoints(int strength);
	int getStrengthPoints() const;

	void setDexterityPoints(int dexterity);
	int getDexterityPoints() const;

	void setSleepTime(int sleep);
	int getSleepTime() const;

	bool isAsleep() const;
	void decreaseSleepTime();


	void setType(string t);
	string getType() const;




private:
	int xPos;
	int yPos;
	int hitPoints;
	string weapon;
	int armorPoints;
	int strengthPoints;
	int dexterityPoints;
	int sleepTime;
	int maxHitPoints;
	string type;

};



#endif
