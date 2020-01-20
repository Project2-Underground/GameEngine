#include "Item.h"
#include "Game.h"

Item::Item(std::string name) {
	interactType = PICKUP;
	object_name = name;
}

void Item::action() {
	// to be picked up by the player
	if (display) {
		Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
		player->anim->Play("Pickup", false);
		player->inventory->addItem(this);
		this->col->enable = false;
	}
	display = false;
}

void Item::SetInventoryTexture(std::string path) {
	inventory_texture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

SeparatableItem::SeparatableItem(std::vector<Item*> items) :Item() {
	for (Item* i : items)
		this->items.push_back(i);
}

void SeparatableItem::Separate() {
	// add the new items to the inventory
	Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
	for (Item* i : items) {
		player->inventory->addItem(i);
	}
	// remove *this from the inventory
	player->inventory->removeItem(this);
}

CombinableItem::CombinableItem(std::string itc, Item* ci) :Item() {
	itemToCombine = itc;
	combinedItem = ci;
}

bool CombinableItem::Combine(Item* item) {
	if (item->object_name == itemToCombine) {
		Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
		// add combinedItem to the inventory
		player->inventory->addItem(combinedItem);
		// remove item and *this from the inventory
		player->inventory->removeItem(item);
		player->inventory->removeItem(this);
		return true;
	}
	return false;
}