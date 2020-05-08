#pragma once

#include <string>
#include <vector>

enum ItemType {
	SEPARATABLE = 0,
	COMBINABLE,
	NONE
};

class Item {
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

class SeparatableItem :public Item {
	// the items that will be separated into
	std::vector<std::string> sitems;
public:
	SeparatableItem(std::string, std::vector<std::string>);
	void action();
};

class CombinableItem :public Item {
	std::string itemToCombine;			// item that can be combined with
	std::string scombinedItem;			// item after combined
public:
	Item* selectedItem;

	CombinableItem(std::string, std::string itc, std::string ci);
	void action();
};
