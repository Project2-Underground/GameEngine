#pragma once

#include "Item.h"
#include "UIObject.h"

class Inventory {
	vector<Item> items;
	vector<UIObject> InventoryBoxes;
public:
	Inventory();	// initialize the inventory boxes
	void addItem(Item item);
	void removeItem(Item item);
	void displayItem();
};