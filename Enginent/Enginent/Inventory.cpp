#include "Inventory.h"

Inventory::Inventory(int numOfInventory, glm::vec3 arrOfPos[], int boxSize) {
	itemSize = numOfInventory;
	// create UIObject and get position of each set from game.h
	for (int i = 0; i < itemSize; i++) {
		InventoryBox *tmpObj =  new InventoryBox();
		tmpObj->SetPosition(arrOfPos[i]);
		tmpObj->SetSize(boxSize, boxSize);
		InventoryBoxes.push_back(tmpObj);
	}
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
		if (*(ib->GetItem()) == *item) {
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