#pragma once

#include "InteractObj.h"
#include "Collider.h"

class Door : public InteractableObj {
	IneractTypeList interactType = DOOR;
	std::string nextRoom;
	std::string nextDoor;
	std::string item_to_unlock;
public:
	bool open;
	Door(std::string, std::string);
	Door() {};
	void SetKey(std::string item_to_unlock);
	void action();
	void Unlock(InteractableObj* item);
	void Unlock() { open = true; };
};