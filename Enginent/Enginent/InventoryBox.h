#pragma once

#include "UIObject.h"
#include "Item.h"

class InventoryBox :public UIObject {
	Item* item;
public:
	InventoryBox();
	void SetItem(Item* item);
	void RemoveItem();
	void Render();
	Item* GetItem();
	~InventoryBox();
};