#include "Item.h"
#include "Game.h"

Item::Item(std::string name) {
	itemName = name;
}

void Item::action() {
	// to be picked up by the player
	Game::GetInstance()->getPlayer()->inventory->addItem(this);
}

bool Item::operator==(const Item& item) {
	if (this->itemName == item.itemName) {
		return true;
	}
	return false;
}
