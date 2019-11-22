#pragma once
#include "InteractObj.h"

class Item : public InteractableObj {
	// description: show on screen when interact with the item
	unsigned int Inventory_texture;
public:
	std::string itemName;
	Item() {};
	Item(std::string name);
	void SetInventoryTexture(std::string path);
	void action();
	bool operator==(const Item& item);
};
