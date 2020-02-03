#pragma once

#include "InventoryBox.h"

#define INVENTORY_SIZE 9
#define POPUP_SPEED 10.0f

class Inventory {
	UIObject* tab;
	Collider* popArea;
	vector<InventoryBoxButton*> InventoryBoxes;

	CombinableItem* itemHolder;

	float boxOffset;
	bool move;
	int direction;
	float minHeight;
	float maxHeight;
public:
	Inventory();
	void Update();
	void Render();
	
	void LeftClick(int x, int y);
	void AddItem(Item *item);
	void RemoveItem(Item *item);
	void SetAllBoxesPos(float);

	void SeparateItem(Item* item);
	void CombineItem(Item* item);


	int GetSize() { return INVENTORY_SIZE; }
	//void displayItem();
	~Inventory();
	InventoryBoxButton* GetInventoryBox(int index);
};