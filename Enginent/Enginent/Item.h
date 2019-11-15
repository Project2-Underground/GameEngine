#pragma once
#include "InteractObj.h"

class Item : public InteractableObj {
	// description: show on screen when interact with the item
public:
	std::string itemName;
	Item() {};
	Item(std::string name);
	virtual void action();
	bool operator==(const Item& item);
};
