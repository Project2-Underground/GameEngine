#pragma once

#include "InventoryBox.h"

#define INVENTORY_SIZE 9
#define POPUP_SPEED 10.0f

class Inventory {
	UIObject* tab;
	Collider* popArea;
	vector<InventoryBoxButton*> InventoryBoxes;
	ChangeMouseActionTypeButton* separateButton;
	ChangeMouseActionTypeButton* combineButton;

	Item* other;

	float boxOffset;
	bool move;
	int direction;
	float minHeight;
	float maxHeight;
	bool triggeredOpen;
public:
	Inventory();
	void Update();
	void Render();
	
	void LeftClick(float x, float y);
	void LeftRelease(float x, float y);
	void AddItem(Item *item);
	void RemoveItem(Item *item);
	void RemoveItem(std::string);
	void SetAllBoxesPos(float);

	void SeparateItem(Item* item);
	void CombineItem(Item* item);
	void SelectItem(Item* item);
	void UnselectItem();
	bool IsItemInInventory(std::string);
	Item* GetSelectedItem() { return other; }


	int GetSize() { return (int)InventoryBoxes.size(); }
	//void displayItem();
	~Inventory();
	InventoryBoxButton* GetInventoryBox(int index);
};