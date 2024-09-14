#include "GameObject.h"

#include <string>
#include <vector>

GameObject::GameObject(const string& name) : name(name) {}


string GameObject::getName() const {
	return name;
}

