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
	virtual void checkColliderPressed(float x, float y);
	virtual void checkColliderReleased(float x, float y);
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

class OpenLoadSaveWindow :public Button {
public:
	OpenLoadSaveWindow(std::string texture) : Button(texture) {}
	void action();
};

class PhoneOpenButton : public Button {
public:
	PhoneOpenButton(std::string texture) : Button(texture) {}
	void action();
};

class PhoneAppsButton :public Button {
	int appType;
	unsigned int notiTexture;
	bool notice;
public:
	PhoneAppsButton(std::string texture) : Button(texture) {}
	void checkColliderPressed(float x, float y);
	void checkColliderReleased(float x, float y);
	void SetNotiTexture(std::string);
	void Notice(bool b) { if (b) SetTexture(notiTexture); notice = b; }
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

class WindowClose :public Button {
public:
	WindowClose(std::string texture) :Button(texture) {}
	void action();
};

class ChangeMouseActionTypeButton : public Button {
	int type;
public:
	ChangeMouseActionTypeButton(std::string texture, int type) :Button(texture) { this->type = type; };
	void checkColliderPressed(float x, float y);
	void checkColliderReleased(float x, float y);
	void updateButton(float x, float y);
	void Reset();
	void action();
};

class ClosePuzzleButton :public Button {
public:
	ClosePuzzleButton(std::string texture) :Button(texture) {}
	void action();
};

class SaveLoadGameButton :public Button {
	std::string filename;
	std::string saveLevel;		// text telling player has saved here
public:
	SaveLoadGameButton(std::string texture, std::string f) :Button(texture) { filename = f; }
	void SetSaveLevel(std::string s) { saveLevel = s; }
	void action();
};

class OpenPauseWindowButton :public Button {
public:
	OpenPauseWindowButton(std::string texture) :Button(texture) {}
	void action();
};