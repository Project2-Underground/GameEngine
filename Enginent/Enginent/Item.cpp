#include "Item.h"
#include "Game.h"

Item::Item(std::string name) {
	this->name = name;
}

void Item::SetInventoryTexture(std::string path) {
	iTexture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

void Item::SetViewTexture(std::string path) {
	viewTexture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

SeparatableItem::SeparatableItem(std::string name, std::vector<Item*> items) :Item(name) {
	for (Item* i : items)
		this->items.push_back(i);
}

void SeparatableItem::action() {
	// add the new items to the inventory
	Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
	for (Item* i : items) {
		player->inventory->addItem(i);
	}
	// remove *this from the inventory
	player->inventory->removeItem(this);
}

CombinableItem::CombinableItem(std::string name, std::string itc, Item* ci) :Item(name) {
	itemToCombine = itc;
	combinedItem = ci;
}

void CombinableItem::action() {
	if (selectedItem->name == itemToCombine) {
		Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
		// add combinedItem to the inventory
		// remove item and *this from the inventory
		player->inventory->removeItem(selectedItem);
		player->inventory->removeItem(this);
		player->inventory->addItem(combinedItem);
	}
}

bool Item::operator==(const Item& item) {
	if (this->name == item.name) {
		return true;
	}
	return false;
}