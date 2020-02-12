#pragma once
#include "Button.h"

class InventoryBoxButton : public Button {
protected:
	Item* item;
	UIObject itemDisplay;
public:
	InventoryBoxButton(std::string texture);
	void Reset();
	void action();
	void checkCollider(float x, float y);
	void RemoveItem();
	void RenderItem();
	void SetAllPosition(glm::vec3);
	Item* GetItem();
	void SetItem(Item* item);
	~InventoryBoxButton();
};