#include "InventoryBox.h"
#include "Game.h"
#include "MouseInput.h"

InventoryBoxButton::InventoryBoxButton(std::string texture):Button(texture) {
	item = nullptr;
	itemDisplay.SetSize(25.0f, 25.0f);
}

void InventoryBoxButton::SetItem(Item* item) {
	this->item = item;
	itemDisplay.SetTexture(item->GetInventoryTexture());
}

void InventoryBoxButton::action() {
	Inventory* inventory = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
	if (!IsSelected()) {
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
	else {
		inventory->UnselectItem();
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

void InventoryBoxButton::Reset() {
	pressAvailable = true;
	SetTogglePress(false);
	SetTexture(normalTexture);
}

void InventoryBoxButton::checkCollider(float x, float y) {
	if (pressAvailable) {
		if (this->col->isClicked(x, y)) {
			if (item) {
				SetTexture(pressTexture);
				action();
			}
			else
			{
				SetTexture(normalTexture);
			}
		}
	}
	else {
		if (this->col->isClicked(x, y))
			action();
	}
}

void ChangeMouseActionTypeButton::action() {
	if (!IsSelected()) {
		std::cout << (type == SEPARATE_ACTION ? "separate" : "combine") << " clicked\n";
		MouseInput::GetInstance()->SetActionEventType((ActionEvent)type);
		SetTogglePress(true);
	}
	else {
		((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory()->UnselectItem();
	}
}

void ChangeMouseActionTypeButton::Reset() {
	pressAvailable = true;
	SetTogglePress(false); 
	SetTexture(normalTexture);
}

void ChangeMouseActionTypeButton::updateButton(float x, float y)
{
	if (togglePressed) {
		SetTexture(pressTexture);
	}
	else if (this->col->isClicked(x, y))
	{
		SetTexture(hoverTexture);
	}
	else
	{
		SetTexture(normalTexture);
	}
}

void ChangeMouseActionTypeButton::checkCollider(float x, float y)
{
	if (pressAvailable) {
		if (this->col->isClicked(x, y))
		{
			((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory()->UnselectItem();
			SetTexture(pressTexture);
			action();
		}
		else
		{
			SetTexture(normalTexture);
		}
	}
	else {
		if (col->isClicked(x, y)) {
			action();
		}
	}
}