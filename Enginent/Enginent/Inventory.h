#pragma once

#include "InventoryBox.h"

#define INVENTORY_SIZE 9
#define POPUP_SPEED 10

class Inventory {
	UIObject* tab;
	Collider* popArea;
	vector<InventoryBox*> InventoryBoxes;

	float boxOffset;
	bool move;
	int direction;
	int minHeight;
	int maxHeight;
public:
	Inventory();
	void Update();
	void Render();
	
	void addItem(Item *item);
	void removeItem(Item *item);
	void SetAllBoxesPos(float);
	int GetSize() { return INVENTORY_SIZE; }
	//void displayItem();
	~Inventory();
	InventoryBox* GetInventoryBox(int index);
};