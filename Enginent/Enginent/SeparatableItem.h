#pragma once

#include "Item.h"

class SeparatableItem :public Item {
	// the items that will be separated into
	Item* item1;
	Item* item2;
public:
	SeparatableItem(Item* item1, Item* item2);
	void Separate();
};