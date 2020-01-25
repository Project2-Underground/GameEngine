#pragma once

#include <string>
#include <vector>

enum ItemType {
	SEPARATABLE = 0,
	COMBINABLE,
	NONE
};

class Item {
	unsigned int inventory_texture;
public:
	std::string name;

	Item(std::string name);
	void SetInventoryTexture(std::string path);
	unsigned int GetInventoryTexture() { return inventory_texture; }

	bool operator==(const Item& item);
	virtual void action() {};
};

class SeparatableItem :public Item {
	// the items that will be separated into
	std::vector<Item*> items;
public:
	SeparatableItem(std::string, std::vector<Item*>);
	void action();
};

class CombinableItem :public Item {
	std::string itemToCombine;			// item that can be combined with
	Item* combinedItem;			// item after combined
public:
	Item* selectedItem;

	CombinableItem(std::string, std::string itc, Item* ci);
	void action();
};
