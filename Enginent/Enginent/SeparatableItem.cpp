#include "SeparatableItem.h"
#include "Game.h"

SeparatableItem::SeparatableItem(Item* item1, Item* item2) {
	this->item1 = item1;
	this->item2 = item2;
}

void SeparatableItem::Separate() {
	// add the new items to the inventory
	Game::GetInstance()->getPlayer()->inventory->addItem(item1);
	Game::GetInstance()->getPlayer()->inventory->addItem(item2);
	// remove *this from the inventory
	Game::GetInstance()->getPlayer()->inventory->removeItem(this);
}