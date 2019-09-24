#pragma once

#include "Item.h"

class Inventory {
	vector<Item> items;
	vector<ImageObject> displayBoxes;
public:
	Inventory();	// initialize the displat boxes
	void addItem(Item item);
	void removeItem(Item item);
	void displayItem();
};