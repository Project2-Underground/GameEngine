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
	Inventory* inventory = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
	for (Item* i : items) {
		inventory->AddItem(i);
	}
	// remove *this from the inventory
	inventory->RemoveItem(this);
}

CombinableItem::CombinableItem(std::string name, std::string itc, Item* ci) :Item(name) {
	itemToCombine = itc;
	combinedItem = ci;
}

void CombinableItem::action() {
	if (selectedItem->name == itemToCombine) {
		Inventory* inventory = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
		// add combinedItem to the inventory
		// remove item and *this from the inventory
		inventory->RemoveItem(selectedItem);
		inventory->RemoveItem(this);
		inventory->AddItem(combinedItem);
	}
	else {
		std::cout << "Combine fail\n";
	}
}

bool Item::operator==(const Item& item) {
	if (this->name == item.name) {
		return true;
	}
	return false;
}