#pragma once
#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class GameObject {
public:
	GameObject(const string& name);
	virtual ~GameObject() {}

	string getName() const;

	virtual void performAction() = 0;


private:
	string name;

};

class idol : public GameObject {
public:
	idol() : GameObject("Golden Idol") {}
	virtual ~idol() {}
};


#endif