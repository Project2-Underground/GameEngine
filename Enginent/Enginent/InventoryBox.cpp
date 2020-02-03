#include "InventoryBox.h"
#include "Game.h"
#include "MouseInput.h"

InventoryBoxButton::InventoryBoxButton(std::string texture):ActionButton(texture) {
	item = nullptr;
	itemDisplay.SetSize(25.0f, 25.0f);
}

void InventoryBoxButton::SetItem(Item* item) {
	this->item = item;
	itemDisplay.SetTexture(item->GetInventoryTexture());
}

void InventoryBoxButton::action() {
	Inventory* inventory = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
	switch (MouseInput::GetInstance()->GetActionEvent())
	{
	case SEPARATE_ACTION:
		inventory->SeparateItem(item);
		break;
	case COMBINE_ACTION:
		inventory->CombineItem(item);
		break;
	default:
		break;
	}
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

void ChangeMouseActionTypeButton::action() {
	MouseInput::GetInstance()->SetActionEventType((ActionEvent)type);
}