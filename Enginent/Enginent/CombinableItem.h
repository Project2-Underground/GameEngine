#pragma once

#include "Item.h"

class CombinableItem :public Item {
	Item* itemToCombine;			// item that can be combined with
	Item* combinedItem;			// item after combined
public:
	CombinableItem(Item* itc, Item* ci);
	bool Combine(Item* item);	// return false if combination failed
};