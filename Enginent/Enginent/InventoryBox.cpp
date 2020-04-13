#include "InventoryBox.h"
#include "Game.h"
#include "MouseInput.h"
#include "TimeSystem.h"

InventoryBoxButton::InventoryBoxButton(std::string texture) :Button(texture) {
	item = nullptr;
	itemDisplay.SetSize(100.0f, -100.0f);
	clickIntervalTime = DOUBLE_CLICK_TIME;
}

void InventoryBoxButton::SetItem(Item* item) {
	this->item = item;
	itemDisplay.SetTexture(item->GetInventoryTexture());
}

void InventoryBoxButton::action() {
	Inventory* inventory = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
	if (!IsSelected() && item) {
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
		SetTogglePress(false);
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
	SetTogglePress(false);
	SetTexture(normalTexture);
}

void InventoryBoxButton::checkColliderPressed(float x, float y) {
	if (pressAvailable && item) {
		if (this->col->isClicked(x, y)) {
			MouseInput::GetInstance()->SetCurrentButtonPressed(this);
		}
	}
	else if (item) {
		std::cout << clickIntervalTime << std::endl;
		if (clickIntervalTime < DOUBLE_CLICK_TIME) {
			clickIntervalTime = DOUBLE_CLICK_TIME;
			ViewWindow* vw = ViewWindow::GetInstance();
			vw->Open();
			vw->SetViewItem(item);
			action();
		}
	}
}
void InventoryBoxButton::updateButton(float x, float y) {
	if(!pressAvailable)
		clickIntervalTime += TimeSystem::instance()->GetTimeBetweenFrame();
}

void InventoryBoxButton::checkColliderReleased(float x, float y) {
	if (pressAvailable && item) {
		if (this->col->isClicked(x, y)) {
			if (MouseInput::GetInstance()->GetCurrentButtonPressed() == this) {
				SetTexture(pressTexture);
				clickIntervalTime = 0;
				action();
			}
		}
	}
	else {
		if (this->col->isClicked(x, y))
			if (MouseInput::GetInstance()->GetCurrentButtonPressed() == this)
				action();
	}
}

void ChangeMouseActionTypeButton::action() {
	if (!IsSelected()) {
		//std::cout << (type == SEPARATE_ACTION ? "separate" : "combine") << " clicked\n";
		MouseInput::GetInstance()->SetActionEventType((ActionEvent)type);
		SetTogglePress(true);
	}
	else {
		((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory()->UnselectItem();
	}
}

void ChangeMouseActionTypeButton::Reset() {
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

void ChangeMouseActionTypeButton::checkColliderPressed(float x, float y)
{
	if (pressAvailable) 
		if (this->col->isClicked(x, y)) 
			MouseInput::GetInstance()->SetCurrentButtonPressed(this);
}

void ChangeMouseActionTypeButton::checkColliderReleased(float x, float y) {
	if (pressAvailable) {
		if (this->col->isClicked(x, y))
			if (MouseInput::GetInstance()->GetCurrentButtonPressed() == this) {
				((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory()->UnselectItem();
				SetTexture(pressTexture);
				action();
			}
		else
		{
			SetTexture(normalTexture);
		}
	}
	else 
		if (col->isClicked(x, y)) 
			if (MouseInput::GetInstance()->GetCurrentButtonPressed() == this) 
				action();

}