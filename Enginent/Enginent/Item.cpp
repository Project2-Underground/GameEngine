#include "Item.h"
#include "Game.h"
#include "TextBox.h"

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
void Item::AddChangeOtherDialogueAfterUsedWithObj(std::string objName, std::string otherObjName, std::string otherItem, std::string dialogue) {
	NextDialogueChange tmp;
	tmp.dialogue = dialogue;
	tmp.otherItemName = otherItem;
	tmp.otherObjName = otherObjName;
	std::vector<NextDialogueChange> tmpvec;
	changeOtherDialogueAfterUsedWithObj.insert(std::pair<std::string, std::vector<NextDialogueChange>>(objName, tmpvec));
	changeOtherDialogueAfterUsedWithObj[objName].push_back(tmp);
}
void Item::DialogueHandle(std::string objName) {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	for (auto obj : changeOtherDialogueAfterUsedWithObj) {
		if (obj.first == objName) {
			for (auto obj2 : obj.second) {
				Item* i = gs->FindItem(obj2.otherItemName);
				i->ChangeDialogueAfterUsedWithObj(obj2.otherObjName, obj2.dialogue);
			}
		}
	}
	TextBox::GetInstance()->setText(GetDialogueAfterUseWith(objName));
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
	if (inventory->FreeSlot() >= itemsAfterSeparated.size()) {
		if (itemsAfterSeparated.size() != 0) {
			inventory->RemoveItem(this);
			for (auto i : itemsAfterSeparated) {
				inventory->AddItem(gs->FindItem(i));
			}
		}
	}
	else {
		// settext inventory full
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

bool Item::operator==(const Item& item) {
	if (this->name == item.name) {
		return true;
	}
	return false;
}