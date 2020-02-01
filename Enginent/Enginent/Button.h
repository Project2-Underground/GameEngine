#pragma once
#include "InteractObj.h"
#include "UIObject.h"
#include "Item.h"
#include <iostream>

class Button : public UIObject {
	protected:
		unsigned int normalTexture;
		unsigned int hoverTexture;
		unsigned int pressTexture;
	public:
		Button() {};
		Button(std::string, std::string, std::string);
		void updateButton(int, int);
		virtual void action() = 0;
		virtual void checkCollider(int x, int y);

		void SetActionTexture(std::string, std::string, std::string);
};

class Exit_Button : public Button {
	public:
		Exit_Button(std::string normal, ::string hover, std::string press) : Button(normal, hover, press) {};
		void action();
};

class SwitchScene_Button : public Button {
public:
	SwitchScene_Button(std::string normal, std::string hover, std::string press) : Button(normal, hover, press) {};
	void action();
};

class PhoneAppsButton :public Button {
	int appType;
public:
	PhoneAppsButton(std::string texture) { SetTexture(texture); }
	void action();
	void SetApp(int type) { appType = type; }
	void checkCollider(int x, int y);
};

class ActionButton :public Button {
public:
	ActionButton(std::string texture) { SetTexture(texture); }
	void action();
	void checkCollider(int x, int y);
};

class PhoneExitButton : public ActionButton {
public:
	PhoneExitButton(std::string texture) :ActionButton(texture) {};
	void action();
};

class PhoneNextButton : public ActionButton {
public:
	PhoneNextButton(std::string texture) :ActionButton(texture) {};
	void action();
};

class PhoneBackButton : public ActionButton {
public:
	PhoneBackButton(std::string texture) :ActionButton(texture) {};
	void action();
};

class PhoneHomeButton : public ActionButton {
public:
	PhoneHomeButton(std::string texture) :ActionButton(texture) {};
	void action();
};

class SeparateButton : public ActionButton {
public:
	SeparateButton(std::string texture) :ActionButton(texture) {};
	void action();
};

class CombineButton : public ActionButton {
public:
	CombineButton(std::string texture) :ActionButton(texture) {};
	void action();
};

class ViewWindowClose :public ActionButton {
public:
	ViewWindowClose(std::string texture) :ActionButton(texture) {}
	void action();
};

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