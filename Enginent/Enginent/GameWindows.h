#pragma once    

#include "Button.h"
#include "Item.h"

class GameWindow {
protected:
	UIObject* bgWindow;
	bool display;
	bool trigger;
	Button* closeButton;
public:
	virtual void Update();
	virtual void Init(int width, int height) = 0;
	virtual void Render() = 0;
	virtual void LeftClick(float, float) = 0;
	virtual void LeftRelease(float, float) = 0;
	virtual void UpdateMouseButton(glm::vec3) = 0;

	virtual void Close();
	virtual void Open();
	bool IsOpen() { return display; }
};

class ViewWindow :public GameWindow{
	static ViewWindow* instance;
	UIObject* viewItem;
	ViewWindow();
	// textbox to display description
public:
	static ViewWindow* GetInstance();

	~ViewWindow();

	void Init(int width, int height);
	void Render();
	void LeftClick(float, float);
	void LeftRelease(float, float);
	void UpdateMouseButton(glm::vec3);

	void SetViewItem(Item* item);
	void SetViewItem(ViewObj*);
	void SetText();
};

class SaveLoadWindow :public GameWindow {
	//SaveLoadGameButton* save;
	static SaveLoadWindow* instance;

	SaveLoadWindow();
public:
	static SaveLoadWindow* GetInstance();

	~SaveLoadWindow();

	void Init(int width, int height);
	void Render();
	void LeftClick(float, float);
	void LeftRelease(float, float);
	void UpdateMouseButton(glm::vec3);

	std::vector<SaveLoadGameButton*> saveButtons;
};

class PauseWindow: public GameWindow {
	// buttons
	static PauseWindow* instance;

	// windows
	std::vector<GameWindow*> otherWindows;

	// buttons
	SettingButton* setting;
	OpenLoadSaveWindow* load;
	MainMenuButton* menu;
	PauseWindow();
public:
	static PauseWindow* GetInstance();

	~PauseWindow();
	void Init(int width, int height);
	void Render();
	void LeftClick(float, float);
	void LeftRelease(float, float);
	void UpdateMouseButton(glm::vec3);

	void Close();
	void Open();
};

class SettingWindow : public GameWindow {
	static SettingWindow* instance;

	SoundVolumeButton* masterVolUp;
	SoundVolumeButton* masterVolDown;
	SoundVolumeButton* bgmVolUp;
	SoundVolumeButton* bgmVolDown;
	SoundVolumeButton* sfxVolUp;
	SoundVolumeButton* sfxVolDown;

	SoundMuteButton* masterMute;
	SoundMuteButton* bgmMute;
	SoundMuteButton* sfxMute;
public:
	static SettingWindow* GetInstance();
	SettingWindow();
	~SettingWindow();
	void Init(int width, int height);
	void Update();
	void Render();
	void LeftClick(float, float);
	void LeftRelease(float, float);
	void UpdateMouseButton(glm::vec3);
};