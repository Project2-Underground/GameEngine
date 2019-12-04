#include "Item.h"
#include "Game.h"

Item::Item(std::string name) {
	interactType = PICKUP;
	object_name = name;
}

void Item::action() {
	// to be picked up by the player
	if (display) {
		SetTexture(Inventory_texture);
		Game::GetInstance()->getPlayer()->anim->Play("Pickup", false);
		Game::GetInstance()->getPlayer()->inventory->addItem(this);
		this->col->enable = false;
	}
	display = false;
}

void Item::SetInventoryTexture(std::string path) {
	Inventory_texture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

SeparatableItem::SeparatableItem(std::vector<Item*> items):Item(){
	this->items = items;
}

void SeparatableItem::Separate() {
	// add the new items to the inventory
	for (Item* i : items) {
		Game::GetInstance()->getPlayer()->inventory->addItem(i);
	}
	// remove *this from the inventory
	Game::GetInstance()->getPlayer()->inventory->removeItem(this);
}

CombinableItem::CombinableItem(std::string itc, Item* ci) :Item() {
	itemToCombine = itc;
	combinedItem = ci;
}

bool CombinableItem::Combine(Item* item) {
	if (item->object_name == itemToCombine) {
		// add combinedItem to the inventory
		Game::GetInstance()->getPlayer()->inventory->addItem(combinedItem);
		// remove item and *this from the inventory
		Game::GetInstance()->getPlayer()->inventory->removeItem(item);
		Game::GetInstance()->getPlayer()->inventory->removeItem(this);
		return true;
	}
	return false;
}