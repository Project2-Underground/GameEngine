#pragma once
#include "Button.h"

constexpr auto DOUBLE_CLICK_TIME = 500;

class InventoryBoxButton : public Button {
protected:
	Item* item;
	UIObject itemDisplay;
	double clickIntervalTime;
public:
	InventoryBoxButton(std::string texture);
	void Reset();
	void action();
	void updateButton(float, float);
	void checkColliderPressed(float x, float y);
	void checkColliderReleased(float x, float y);
	void RemoveItem();
	void RenderItem();
	void SetAllPosition(glm::vec3);
	Item* GetItem();
	void SetItem(Item* item);
	~InventoryBoxButton();
};