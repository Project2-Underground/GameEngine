#pragma once
#include "Button.h"

class InventoryBoxButton : public ActionButton {
protected:
	Item* item;
	UIObject itemDisplay;
public:
	InventoryBoxButton(std::string texture);
	void action();
	void RemoveItem();
	void RenderItem();
	void SetAllPosition(glm::vec3);
	Item* GetItem();
	void SetItem(Item* item);
	~InventoryBoxButton();
};