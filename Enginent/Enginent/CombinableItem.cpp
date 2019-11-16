#include "CombinableItem.h"
#include "Game.h"


CombinableItem::CombinableItem(Item* itc, Item* ci) {
	itemToCombine = itc;
	combinedItem = ci;
}

bool CombinableItem::Combine(Item* item) {
	if (*item == *itemToCombine) {
		// add combinedItem to the inventory
		Game::GetInstance()->getPlayer()->inventory->addItem(combinedItem);
		// remove item and *this from the inventory
		Game::GetInstance()->getPlayer()->inventory->removeItem(itemToCombine);
		Game::GetInstance()->getPlayer()->inventory->removeItem(this);
		return true;
	}
	return false;
}