#include "Item.h"
#include "Game.h"

Item::Item(std::string name) {
	this->name = name;
}

void Item::SetInventoryTexture(std::string path) {
	iTexture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

void Item::SetViewTexture(std::string path) {
	viewTexture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}
void Item::SetSize(float x, float y) {
	width = x;
	height = -y;
	aspect = x / y;
	iwidth = x;
	iheight = -y;
	iaspect = x / y;
}
void Item::SetISize(float x, float y) {
	iwidth = x;
	iheight = -y;
	iaspect = x / y;
}
void Item::AddSeparatedItem(std::string separatedItem) {
	itemsAfterSeparated.push_back(separatedItem);
}
void Item::AddItemsToCombine(std::string itemToCombine, std::string combineResult) {
	itemsAfterCombined.insert(std::pair<std::string, std::string>(itemToCombine, combineResult));
}
void Item::AddDialogueAfterUsedWithObj(std::string objName, std::string dialogueName) {
	dialogueAfterUsedWithObj.insert(std::pair<std::string, std::string>(objName, dialogueName));
}
void Item::ChangeDialogueAfterUsedWithObj(std::string objName, std::string dialogueName) {
	dialogueAfterUsedWithObj[objName] = dialogueName;
}
std::string Item::GetDialogueAfterUseWith(std::string objName) {
	for (auto obj : dialogueAfterUsedWithObj) {
		if (obj.first == objName)
			return obj.second;
	}
	return "";
}
void Item::Separate() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	Inventory* inventory = gs->GetInventory();
	if (itemsAfterSeparated.size() != 0) {
		inventory->RemoveItem(this);
		for (auto i : itemsAfterSeparated) {
			inventory->AddItem(gs->FindItem(i));
		}
	}
}
void Item::Combine(Item* other) {
	for (auto i : itemsAfterCombined) {
		if (i.first == other->name) {
			GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
			Inventory* inventory = gs->GetInventory();
			inventory->RemoveItem(other);
			inventory->RemoveItem(this);
			inventory->AddItem(gs->FindItem(i.second));
			break;
		}
	}
}
//SeparatableItem::SeparatableItem(std::string name, std::vector<std::string> items) :Item(name) {
//	for (auto i : items)
//		this->itemsAfterSeparated.push_back(i);
//}
//
//void SeparatableItem::action() {
//	// add the new items to the inventory
//	Inventory* inventory = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
//	for (auto i : itemsAfterSeparated) {
//		inventory->AddItem(((GameScreen*)Game::GetInstance()->GetScreen())->FindItem(i));
//	}
//	// remove *this from the inventory
//	inventory->RemoveItem(this);
//}
//
//CombinableItem::CombinableItem(std::string name, std::string itc, std::string ci) :Item(name) {
//	itemToCombine = itc;
//	scombinedItem = ci;
//}
//
//void CombinableItem::action() {
//	if (other->name == itemToCombine) {
//		Inventory* inventory = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
//		// add combinedItem to the inventory
//		// remove item and *this from the inventory
//		inventory->RemoveItem(other);
//		inventory->RemoveItem(this);
//		inventory->AddItem(((GameScreen*)Game::GetInstance()->GetScreen())->FindItem(scombinedItem));
//	}
//	/*else {
//		std::cout << "Combine fail\n";
//	}*/
//}

bool Item::operator==(const Item& item) {
	if (this->name == item.name) {
		return true;
	}
	return false;
}