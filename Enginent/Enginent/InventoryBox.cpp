#include "Button.h"
#include "Game.h"

InventoryBoxButton::InventoryBoxButton(std::string texture):ActionButton(texture) {
	item = nullptr;
	itemDisplay.SetSize(25.0f, 25.0f);
}

void InventoryBoxButton::SetItem(Item* item) {
	this->item = item;
	itemDisplay.SetTexture(item->GetInventoryTexture());
}

void InventoryBoxButton::action() {
	std::cout << "this box is clicked\n";
	// set this item to selected
}

Item* InventoryBoxButton::GetItem() {
	return item;
}

void InventoryBoxButton::RemoveItem() {
	item = nullptr;
}

void InventoryBoxButton::RenderItem() {
	if (item)
		Game::GetInstance()->GetRenderer()->Render(&itemDisplay);
}

void InventoryBoxButton::SetAllPosition(glm::vec3 pos) {
	SetPosition(pos);
	col->setNewPos(pos);
	itemDisplay.SetPosition(pos);
}

InventoryBoxButton::~InventoryBoxButton() {
	delete item;
}