#include "SeparatableItem.h"


SeparatableItem::SeparatableItem(Item* item1, Item* item2) {
	this->item1 = item1;
	this->item2 = item2;
}

void SeparatableItem::Separate() {
	// add the new items to the inventory
	// remove *this from the inventory
}