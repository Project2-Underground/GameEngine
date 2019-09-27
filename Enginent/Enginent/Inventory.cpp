#include "Inventory.h"
#include "Game.h"


Inventory::Inventory() {
	// create UIObject and get position of each set from game.h
}

void Inventory::addItem(Item item) {
	items.push_back(item);
}

void Inventory::removeItem(Item item) {
	for (int i = 0; i < items.size(); i++) {
		if (items[i] == item) {
			items.erase(items.begin()+i);
			break;
		}
	}
}

void Inventory::displayItem() {
	// put info of items into each boxes
}