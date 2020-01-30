#pragma once

#include "InventoryBox.h"

#define INVENTORY_SIZE 9

class Inventory {
	vector<InventoryBox*> InventoryBoxes;

	bool move;
	int direction;
public:
	Inventory();
	void Update();
	
	void addItem(Item *item);
	void removeItem(Item *item);
	int GetSize() { return INVENTORY_SIZE; }
	//void displayItem();
	~Inventory();
	InventoryBox* GetInventoryBox(int index);
};