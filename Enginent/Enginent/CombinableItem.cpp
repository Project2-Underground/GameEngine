#include "CombinableItem.h"


CombinableItem::CombinableItem(Item* itc, Item* ci) {
	itemToCombine = itc;
	combinedItem = ci;
}

bool CombinableItem::Combine(Item* item) {
	if (*item == *itemToCombine) {
		// add combinedItem to the inventory
		// remove item and *this from the inventory
		return true;
	}
	return false;
}