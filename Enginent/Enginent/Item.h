#pragma once
#include "InteractObj.h"

enum ItemType {
	SEPARATABLE = 0,
	COMBINABLE
};

class Item : public InteractableObj {
	unsigned int Inventory_texture;
public:
	Item() { interactType = PICKUP; };
	Item(std::string name);
	void SetInventoryTexture(std::string path);
	void action();
};

class SeparatableItem :public Item {
	// the items that will be separated into
	std::vector<Item*> items;
public:
	SeparatableItem(std::vector<Item*>);
	void Separate();
};

class CombinableItem :public Item {
	std::string itemToCombine;			// item that can be combined with
	Item* combinedItem;			// item after combined
public:
	CombinableItem(std::string itc, Item* ci);
	bool Combine(Item* item);	// return false if combination failed
};
