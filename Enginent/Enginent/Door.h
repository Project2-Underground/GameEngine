#pragma once

#include "InteractObj.h"
#include "Collider.h"
#include "Item.h"

class Door : public InteractableObj {
	InteractTypeList interactType = DOOR;
	std::string nextRoom;
	std::string nextDoor;
	std::string item_to_unlock;
public:
	bool lock;
	Door(std::string, std::string);
	Door() {};
	void SetKey(std::string item_to_unlock);
	void action();
	void Unlock(Item* item);
};