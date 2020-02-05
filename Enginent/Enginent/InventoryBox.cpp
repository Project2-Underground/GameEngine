#include "InventoryBox.h"
#include "Game.h"
#include "MouseInput.h"

InventoryBoxButton::InventoryBoxButton(std::string texture):ActionButton(texture) {
	pressAvailable = false;
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
		SetTogglePress(true);
		inventory->CombineItem(item);
		break;
	default:
		SetTogglePress(true);
		MouseInput::GetInstance()->SetActionEventType(ITEM_SELECTED_ACTION);
		inventory->SelectItem(item);
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
	itemDisplay.SetPosition(pos);
}

InventoryBoxButton::~InventoryBoxButton() {
	delete item;
}

void ChangeMouseActionTypeButton::action() {
	std::cout << (type == SEPARATE_ACTION ? "separate" : "combine") << " clicked\n";
	MouseInput::GetInstance()->SetActionEventType((ActionEvent)type);
	SetTogglePress(true);
}