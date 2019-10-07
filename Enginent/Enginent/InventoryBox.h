#pragma once

#include "UIObject.h"
#include "Item.h"

class InventoryBox :public UIObject {
	Item* item;
public:
	InventoryBox();
	void SetItem(Item* item);
	Item* GetItem();
	void RemoveItem();
	void Render();
	~InventoryBox();
};