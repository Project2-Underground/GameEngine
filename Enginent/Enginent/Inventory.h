#pragma once

#include "InventoryBox.h"

class Inventory {
	vector<InventoryBox*> InventoryBoxes;
	int itemSize;
public:
	Inventory(int numOfInventory, glm::vec3 arrOfPos[], int boxSize);	// initialize the inventory boxes
	void addItem(Item *item);
	void removeItem(Item *item);
	//void displayItem();
	~Inventory();
	InventoryBox* GetInventoryBox(int index);
};