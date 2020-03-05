#include "Inventory.h"
#include "Game.h"
#include "MouseInput.h"

Inventory::Inventory() {
	triggeredOpen = false;
	move = false;
	direction = 1;
	selectedItem = nullptr;

	float width = (float)Game::GetInstance()->winWidth;
	float height = (float)Game::GetInstance()->winHeight;

	tab = new UIObject();
	tab->SetSize(width, -height * 0.5f);
	minHeight = -height * 0.5f - tab->getSize().y * -0.5f;
	maxHeight = minHeight + tab->getSize().y * -0.3f;

	tab->SetPosition(glm::vec3(0.0f, minHeight, 1.0f));
	tab->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");

	popArea = new Collider(tab);
	popArea->setNewSize(glm::vec3(tab->getSize().x, tab->getSize().y * 1.25f, 1.0f));

	float boxSize = 75.0f;
	float space = width / (float)INVENTORY_SIZE;
	float x = space * 0.5f - width * 0.5f;
	float y = tab->getPos().y + tab->getSize().y * -0.35f;
	boxOffset = tab->getPos().y - y;
	// create UIObject and get position of each set from game.h
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		InventoryBoxButton *tmpObj =  new InventoryBoxButton("Texture/tmp_texture/tmp_inventoryBox.png");
		tmpObj->SetPressTexture("Texture/tmp_texture/tmp_inventoryBoxSelected.png");
		tmpObj->SetPosition(glm::vec3(x, tab->getPos().y - boxOffset, 1.0f));
		tmpObj->SetSize(boxSize, -boxSize);
		tmpObj->SetCollder(new Collider(tmpObj));
		tmpObj->SetAllPosition(tmpObj->getPos());
		InventoryBoxes.push_back(tmpObj);
		x += space;
	}

	separateButton = new ChangeMouseActionTypeButton("Texture/tmp_texture/tmp_separateButton.png", SEPARATE_ACTION);
	separateButton->SetPressTexture("Texture/tmp_texture/tmp_separateButtonPress.png");
	separateButton->SetPosition(glm::vec3(100.0f, 0, 0));
	separateButton->SetSize(100.0f, -50.0f);
	separateButton->SetCollder(new Collider(separateButton));
	combineButton = new ChangeMouseActionTypeButton("Texture/tmp_texture/tmp_combineButton.png", COMBINE_ACTION);
	combineButton->SetPressTexture("Texture/tmp_texture/tmp_combineButtonPress.png");
	combineButton->SetPosition(glm::vec3(-100.0f, 0, 0));
	combineButton->SetSize(100.0f, -50.0f);
	combineButton->SetCollder(new Collider(combineButton));
}

void Inventory::Update() {
	// temporary mouse input
	int x, y;
	SDL_GetMouseState(&x, &y);
	glm::vec3 realPos = Game::GetInstance()->FindMousePosition(x, y);

	move = true;
	if (IsMouseCollide(popArea, realPos.x, realPos.y)) {
		triggeredOpen = true;
		direction = 1;
	}
	else {
		direction = -1;
	}
	if (move && !selectedItem) {
		if (tab->getPos().y >= maxHeight && direction == 1) {
			move = false;
			tab->SetPosition(glm::vec3(tab->getPos().x, maxHeight, 1.0f));
		}
		else if (tab->getPos().y <= minHeight && direction == -1) {
			move = false;
			tab->SetPosition(glm::vec3(tab->getPos().x, minHeight, 1.0f));
		}
		else {
			float vel = POPUP_SPEED * (float)direction;
			tab->SetPosition(glm::vec3(tab->getPos().x, tab->getPos().y + vel, 1.0f));
		}
		SetAllBoxesPos(tab->getPos().y - boxOffset);
		popArea->setNewPos(glm::vec3(popArea->getPosition().x, tab->getPos().y, 1.0f));
	}
}

void Inventory::UnselectItem() {
	if (triggeredOpen) {
		for (auto* ib : InventoryBoxes) {
			ib->Reset();
		}
		separateButton->Reset();
		combineButton->Reset();
		selectedItem = nullptr;
		MouseInput::GetInstance()->ResetActionType();
		triggeredOpen = false;
	}
}
void Inventory::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(tab);
	for (UIObject* ib : InventoryBoxes) {
		renderer->Render(ib);
		((InventoryBoxButton*)ib)->RenderItem();
	}
	renderer->Render(separateButton);
	renderer->Render(combineButton);
}

void Inventory::SeparateItem(Item* item) {
	if (item && dynamic_cast<SeparatableItem*>(item)) {
		dynamic_cast<SeparatableItem*>(item)->action();
	}
	else {
		//std::cout << "Separate fail\n";
	}
	MouseInput::GetInstance()->ResetActionType(); 
	UnselectItem();
}

void Inventory::CombineItem(Item* item) {
	if (item) {
		if (selectedItem) {
			if (dynamic_cast<CombinableItem*>(selectedItem)) {
				//std::cout << "try combine\n";
				CombinableItem* c = ((CombinableItem*)selectedItem);
				c->selectedItem = item;
				c->action();
			}
			else {
				//std::cout << "items cannot be combine\n";
			}
			MouseInput::GetInstance()->ResetActionType();
			UnselectItem();
		}
		else {
			selectedItem = item;
			//std::cout << "selected item to combine\n";
		}
	}
}

void Inventory::SelectItem(Item* item) {
	if (item) {
		//std::cout << "selected an item\n";
		selectedItem = item;
	}
}

InventoryBoxButton* Inventory::GetInventoryBox(int index) {
	return InventoryBoxes[index];
}

void Inventory::LeftClick(float x, float y) {
	for (auto *ib : InventoryBoxes) {
		ib->checkColliderPressed(x, y);
	}
	separateButton->checkColliderPressed(x, y);
	combineButton->checkColliderPressed(x, y);
}

void Inventory::LeftRelease(float x, float y) {
	for (auto* ib : InventoryBoxes) {
		ib->checkColliderReleased(x, y);
	}
	separateButton->checkColliderReleased(x, y);
	combineButton->checkColliderReleased(x, y);
}

void Inventory::AddItem(Item* item) {
	for (InventoryBoxButton *ib : InventoryBoxes) {
		if (ib->GetItem() == nullptr) {
			ib->SetItem(item);
			break;
		}
	}
}

void Inventory::RemoveItem(Item* item) {
	for (InventoryBoxButton *ib : InventoryBoxes) {
		if (ib->GetItem() != nullptr && *(ib->GetItem()) == *item) {
			ib->RemoveItem();
			UnselectItem();
			break;
		}
	}
}

void Inventory::SetAllBoxesPos(float y) {
	for (InventoryBoxButton* ib : InventoryBoxes) {
		ib->SetAllPosition(glm::vec3(ib->getPos().x, y, 1.0f));
	}
	separateButton->SetPosition(glm::vec3(separateButton->getPos().x, y + 50.0f, 1.0f));
	combineButton->SetPosition(glm::vec3(combineButton->getPos().x, y + 50.0f, 1.0f));
}

Inventory::~Inventory() {
	for (InventoryBoxButton *ib : InventoryBoxes) {
		delete ib;
	}
	delete tab;
	delete popArea;
	delete separateButton;
	delete combineButton;
}