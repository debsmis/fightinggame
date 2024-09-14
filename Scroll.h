#pragma once
#ifndef SCROLL_H
#define SCROLL_H

#include "GameObject.h"


class Scroll : public GameObject {
public:
	Scroll(const string& name) : GameObject(name) {}

	void performAction() {}

	virtual ~Scroll() {}
};



class teleportation : public Scroll {
public: 
	teleportation() : Scroll("Scroll of teleporation") {}
};

class armor : public Scroll {
public:
	armor() : Scroll("Scroll of armor") {}
};

class strength : public Scroll {
public:
	strength() : Scroll("Scroll of strength") {}
};

class health : public Scroll {
public:
	health() : Scroll("Scroll of health") {}
};

class dexterity : public Scroll {
public:
	dexterity() : Scroll("Scroll of enhance dexterity") {}
};

#endif