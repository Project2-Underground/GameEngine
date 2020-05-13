#pragma once

#include <string>
#include <vector>
#include <map>

enum ItemType {
	SEPARATABLE = 0,
	COMBINABLE,
	NONE
};

struct NextDialogueChange
{
	std::string otherObjName;
	std::string otherItemName;
	std::string dialogue;
};

class Item {
protected:
	std::vector<std::string> itemsAfterSeparated;
	std::map<std::string, std::string> itemsAfterCombined;
	std::map<std::string, std::string> dialogueAfterUsedWithObj;
	std::map<std::string, std::vector<NextDialogueChange>> changeOtherDialogueAfterUsedWithObj;
	unsigned int iTexture;
	unsigned int viewTexture;
public:
	std::string name;
	std::string dialogue_name;

	Item(std::string name);
	void SetInventoryTexture(std::string path);
	void SetViewTexture(std::string path);
	unsigned int GetInventoryTexture() { return iTexture; }
	unsigned int GetViewTexture() { return viewTexture; }
	void SetSize(float x, float y);
	void SetISize(float x, float y);

	void AddSeparatedItem(std::string separatedItem);
	void AddItemsToCombine(std::string itemToCombine, std::string combineResult);
	void AddDialogueAfterUsedWithObj(std::string objName, std::string dialogueName);
	void ChangeDialogueAfterUsedWithObj(std::string objName, std::string dialogueName);
	void AddChangeOtherDialogueAfterUsedWithObj(std::string objName, std::string otherObjName, std::string otherItem, std::string dialogue);

	void DialogueHandle(std::string objUsedWith);

	std::string GetDialogueAfterUseWith(std::string objName);

	void Separate();
	void Combine(Item* other);

	float width;
	float height;
	float iwidth;
	float iheight;
	float aspect;
	float iaspect;

	bool multipleUse;
	bool operator==(const Item& item);
	virtual void action() {};
};

//class SeparatableItem :public Item {
//	// the items that will be separated into
//public:
//	SeparatableItem(std::string, std::vector<std::string>);
//	void action();
//};
//
//class CombinableItem :public Item {
//	std::string itemToCombine;			// item that can be combined with
//	std::string scombinedItem;			// item after combined
//public:
//
//	CombinableItem(std::string, std::string itc, std::string ci);
//	void action();
//};
