#pragma once

#include "Item.h"

class CombinableItem :public Item {
	Item itemToCombine;			// item that can be combined with
	Item combinedItem;			// item after combined
public:
	bool combine(Item item);	// return false if combination failed
};