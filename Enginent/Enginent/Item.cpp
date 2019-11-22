#include "Item.h"
#include "Game.h"

Item::Item(std::string name) {
	itemName = name;
}

void Item::action() {
	// to be picked up by the player
	if (display) {
		SetTexture(Inventory_texture);
		Game::GetInstance()->getPlayer()->anim->Play("Pickup", false);
		Game::GetInstance()->getPlayer()->inventory->addItem(this);
	}
	display = false;
}

bool Item::operator==(const Item& item) {
	if (this->itemName == item.itemName) {
		return true;
	}
	return false;
}

void Item::SetInventoryTexture(std::string path) {
	Inventory_texture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}