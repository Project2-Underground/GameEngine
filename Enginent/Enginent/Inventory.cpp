#include "Inventory.h"
#include "Game.h"

Inventory::Inventory() {
	move = false;
	direction = 1;

	float boxSize = 50.0f;
	float space = (float)Game::GetInstance()->winWidth / (float)INVENTORY_SIZE;
	float x = space * 0.5f - Game::GetInstance()->winWidth * 0.5f;
	float y = Game::GetInstance()->winHeight * -0.5f + 75.0f;
	// create UIObject and get position of each set from game.h
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		InventoryBox *tmpObj =  new InventoryBox();
		tmpObj->SetPosition(glm::vec3(x, y, 1.0f));
		tmpObj->SetSize(boxSize, -boxSize);
		InventoryBoxes.push_back(tmpObj);
		x += space;
	}
}


void Inventory::Update() {
	if (move) {

	}
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

Inventory::~Inventory() {
	for (InventoryBox *ib : InventoryBoxes) {
		delete ib;
	}
}