#pragma once
#include "InteractObj.h"
#include "UIObject.h"
#include "Item.h"
#include <iostream>

class Button : public UIObject {
protected:
	std::string sound;
	SDL_Color textColor = { 0, 0, 0, 0 };
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
	void Appear();
	void Disappear();

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

class CreditButton :public Button {
	UIObject* creadit;
public:
	CreditButton(std::string texture, UIObject* c) : Button(texture) { creadit = c; };
	void action()
	{
		creadit->SetDisplay(true);
	};
};
class CreditBackButton :public Button {
	UIObject* creadit;
public:
	CreditBackButton(std::string texture, UIObject* c) : Button(texture) { creadit = c; };
	void action()
	{
		creadit->SetDisplay(false);
	};
};

class PhoneOpenButton : public Button {
	unsigned int notiTexture;
public:
	PhoneOpenButton(std::string texture);
	void Init(float sizex, float sizey, glm::vec3 position);
	void action();
	void checkColliderReleased(float x, float y);
	void UpdateButton(bool noti);
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

//class PhoneNextButton : public Button {
//public:
//	PhoneNextButton(std::string texture) :Button(texture) {};
//	void action();
//};

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

class ChatNoteInfoButton : public Button {
	int index;
public:
	TextObject* title;
	ChatNoteInfoButton(std::string texture, std::string _title, int _index, bool noti);
	void action();

	bool hasNewInfo;
};

class ViewWindowCloseButton :public Button {
public:
	ViewWindowCloseButton(std::string texture) :Button(texture) { }
	void action();
};

class SaveLoadWindowCloseButton :public Button {
public:
	SaveLoadWindowCloseButton(std::string texture) :Button(texture) { }
	void action();
};

class PauseWindowCloseButton :public Button {
public:
	PauseWindowCloseButton(std::string texture) :Button(texture) { }
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
	unsigned int noSaveTexture;
public:
	SaveLoadGameButton(std::string texture, std::string f);
	void InitSaveLoadButton();
	void SetSaveLevel(std::string s);
	void updateButton(float, float);
	void checkColliderPressed(float x, float y);
	void checkColliderReleased(float x, float y);
	void action();

	bool hasSaved;
	TextObject* name;
	TextObject* details;
	std::string saveLevel;		// text telling player has saved here
};

class OpenPauseWindowButton :public Button {
public:
	OpenPauseWindowButton(std::string texture) :Button(texture) {}
	void action();
};

class SettingWindowCloseButton :public Button {
public:
	SettingWindowCloseButton(std::string texture) :Button(texture) { }
	void action();
};

class SettingButton :public Button {
public:
	SettingButton(std::string texture) :Button(texture) {}
	void action();
};

class MainMenuButton :public Button {
public:
	MainMenuButton(std::string texture) : Button(texture) {}
	void action();
};

//class SoundVolumeButton : public Button {
//	int type;
//	int direction;
//public:
//	SoundVolumeButton(std::string texture, int t, int d) : Button(texture) { type = t; direction = d; }
//	void action();
//};

class SoundMuteButton : public Button {
	int type;
	unsigned int muteTexture;
public:
	SoundMuteButton(std::string texture, int t) : Button(texture) { type = t; }
	void SetMuteTexture(std::string);
	void updateButton(float, float);
	void checkColliderPressed(float x, float y);
	void checkColliderReleased(float x, float y);
	void action();
};

