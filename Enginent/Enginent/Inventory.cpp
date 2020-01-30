#include "Inventory.h"
#include "Game.h"

Inventory::Inventory() {
	move = false;
	direction = 1;

	float width = (float)Game::GetInstance()->winWidth;
	float height = (float)Game::GetInstance()->winHeight;

	tab = new UIObject();
	tab->SetSize(width, -height * 0.5f);
	minHeight = -height * 0.5f - tab->getSize().y * -0.5f;
	maxHeight = minHeight + tab->getSize().y * -0.3f;
	std::cout << maxHeight << std::endl;

	tab->SetPosition(glm::vec3(0.0f, minHeight, 1.0f));
	tab->SetTexture("Texture/tmp_inventoryBox.png");

	popArea = new Collider(tab);
	popArea->setNewSize(glm::vec3(tab->getSize().x, tab->getSize().y * 1.25f, 1.0f));

	float boxSize = 75.0f;
	float space = width / (float)INVENTORY_SIZE;
	float x = space * 0.5f - width * 0.5f;
	float y = tab->getPos().y + tab->getSize().y * -0.25;
	boxOffset = tab->getPos().y - y;
	// create UIObject and get position of each set from game.h
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		InventoryBox *tmpObj =  new InventoryBox();
		tmpObj->SetPosition(glm::vec3(x, tab->getPos().y - boxOffset, 1.0f));
		tmpObj->SetSize(boxSize, -boxSize);
		InventoryBoxes.push_back(tmpObj);
		x += space;
	}
}

void Inventory::Update() {
	// temporary mouse input
	int x, y;
	SDL_GetMouseState(&x, &y);
	glm::vec3 realPos = Game::GetInstance()->FindMousePosition(x, y);

	move = true;
	if (IsMouseCollide(popArea, realPos.x, realPos.y)) {
		direction = 1;
	}
	else {
		direction = -1;
	}
	if (move) {
		if (tab->getPos().y >= maxHeight && direction == 1) {
			move = false;
			tab->SetPosition(glm::vec3(tab->getPos().x, maxHeight, 1.0f));
		}
		else if (tab->getPos().y <= minHeight && direction == -1) {
			move = false;
			tab->SetPosition(glm::vec3(tab->getPos().x, minHeight, 1.0f));
		}
		else {
			float vel = POPUP_SPEED * direction;
			tab->SetPosition(glm::vec3(tab->getPos().x, tab->getPos().y + vel, 1.0f));
		}
		SetAllBoxesPos(tab->getPos().y - boxOffset);
		popArea->setNewPos(glm::vec3(popArea->getPosition().x, tab->getPos().y, 1.0f));
	}
}

void Inventory::Render() {
	Game::GetInstance()->GetRenderer()->Render(tab);
	for(UIObject* ib:InventoryBoxes)
		Game::GetInstance()->GetRenderer()->Render(ib);
}

InventoryBox* Inventory::GetInventoryBox(int index) {
	return InventoryBoxes[index];
}

void Inventory::addItem(Item* item) {
	for (InventoryBox *ib : InventoryBoxes) {
		if (ib->GetItem() == nullptr) {
			ib->SetItem(item);
			break;
		}
	}
}

void Inventory::removeItem(Item* item) {
	for (InventoryBox *ib : InventoryBoxes) {
		if (ib->GetItem() != nullptr && *(ib->GetItem()) == *item) {
			ib->RemoveItem();
			break;
		}
	}
}

void Inventory::SetAllBoxesPos(float y) {
	for (InventoryBox* ib : InventoryBoxes) {
		ib->SetPosition(glm::vec3(ib->getPos().x, y, 1.0f));
	}
}

Inventory::~Inventory() {
	for (InventoryBox *ib : InventoryBoxes) {
		delete ib;
	}
	delete tab;
	delete popArea;
}