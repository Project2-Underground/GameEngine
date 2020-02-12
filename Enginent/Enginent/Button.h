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
		bool togglePressed;
		bool pressAvailable;
	public:
		Button() {};
		Button(std::string);
		virtual void updateButton(float, float);
		virtual void action() = 0;
		virtual void checkCollider(float x, float y);
		void SetTogglePress(bool b) { togglePressed = b; pressAvailable = !b; }
		bool IsSelected() { return togglePressed; }

		void SetHoverTexture(std::string);
		void SetPressTexture(std::string);
};

class Exit_Button : public Button {
	public:
		Exit_Button(std::string normal, ::string hover, std::string press) : Button(normal) { SetHoverTexture(hover); SetPressTexture(press); }
		void action();
};

class SwitchScene_Button : public Button {
public:
	SwitchScene_Button(std::string normal, std::string hover, std::string press) : Button(normal) { SetHoverTexture(hover); SetPressTexture(press); }
	void action();
};

class PhoneOpenButton : public Button {
public:
	PhoneOpenButton(std::string texture) : Button(texture) {}
	void action();
};

class PhoneAppsButton :public Button {
	int appType;
public:
	PhoneAppsButton(std::string texture) : Button(texture) {}
	void action();
	void SetApp(int type) { appType = type; }
};

class PhoneExitButton : public Button {
public:
	PhoneExitButton(std::string texture) :Button(texture) {};
	void action();
};

class PhoneNextButton : public Button {
public:
	PhoneNextButton(std::string texture) :Button(texture) {};
	void action();
};

class PhoneBackButton : public Button {
public:
	PhoneBackButton(std::string texture) :Button(texture) {};
	void action();
};

class PhoneHomeButton : public Button {
public:
	PhoneHomeButton(std::string texture) :Button(texture) {};
	void action();
};

class ViewWindowClose :public Button {
public:
	ViewWindowClose(std::string texture) :Button(texture) {}
	void action();
};

class ChangeMouseActionTypeButton : public Button {
	int type;
public:
	ChangeMouseActionTypeButton(std::string texture, int type) :Button(texture) { this->type = type;};
	void checkCollider(float x, float y);
	void updateButton(float x, float y);
	void Reset();
	void action();
};