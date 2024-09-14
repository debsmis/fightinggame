#include "Actor.h" 
using namespace std;



Actor::Actor(int row, int col, int hit, int armor, int strength, int dexterity, int maxHit) :
    xPos(row), yPos(col), hitPoints(hit), armorPoints(armor), strengthPoints(strength), dexterityPoints(dexterity), sleepTime(0), maxHitPoints(maxHit) {}


Actor::~Actor() {} // needs to be virtual?

void Actor::setXPos(int x) {
	xPos = x;
}
void Actor::setYPos(int y) {
	yPos = y;
}

int Actor::getXPos() const {
	return xPos;
}
int Actor::getYPos() const {
	return yPos;
}
void Actor::setHitPoints(int hp) {
	hitPoints = hp;
}
int Actor::getHitPoints() const {
	return hitPoints;
}
int Actor::getMaxHit() const {
	return maxHitPoints;
}

void Actor::changeMaxHit(int y) {
	maxHitPoints += y;
}


void Actor::setArmorPoints(int armor) {
	armorPoints = armor;
}
int Actor::getArmorPoints() const {
	return armorPoints;
}

void Actor::setStrengthPoints(int strength) {
	strengthPoints = strength;
}
int Actor::getStrengthPoints() const {
	return strengthPoints;
}

void Actor::setDexterityPoints(int dexterity) {
	dexterityPoints = dexterity;
}
int Actor::getDexterityPoints() const {
	return dexterityPoints;
}

void Actor::setSleepTime(int sleep) {
	sleepTime += sleep;
}

int Actor::getSleepTime() const {
	return sleepTime;
}

bool Actor::isAsleep() const { 
	return sleepTime > 0; 
}

void Actor::decreaseSleepTime() { 
	if (sleepTime > 0) 
		--sleepTime; 
}


void Actor::setType(string t) {
	type = t;
}

string Actor::getType() const {
	return type;
}

