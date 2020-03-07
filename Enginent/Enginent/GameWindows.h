#pragma once    

#include "Button.h"

class GameWindow {
protected:
	UIObject* bgWindow;
	bool display;
	bool trigger;
	WindowCloseButton* closeButton;
public:
	virtual void Update() = 0;
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

	void Update();
	void Init(int width, int height);
	void Render();
	void LeftClick(float, float);
	void LeftRelease(float, float);
	void UpdateMouseButton(glm::vec3);

	void SetViewItem(unsigned int texture);
	void SetText();
};

class SaveLoadWindow :public GameWindow {
	SaveLoadGameButton* save;
	static SaveLoadWindow* instance;

	SaveLoadWindow();
public:
	static SaveLoadWindow* GetInstance();

	~SaveLoadWindow();

	void Update();
	void Init(int width, int height);
	void Render();
	void LeftClick(float, float);
	void LeftRelease(float, float);
	void UpdateMouseButton(glm::vec3);
};

class PauseWindow: public GameWindow {
	// buttons
	static PauseWindow* instance;

	SettingButton* setting;
	OpenLoadSaveWindow* load;
	MainMenuButton* menu;
	PauseWindow();

	bool otherWindowTrigger;
	bool OtherWindowOpens();
public:
	static PauseWindow* GetInstance();

	~PauseWindow();
	void Update();
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

	SoundVolumeButton* bgmVolUp;
	SoundVolumeButton* bgmVolDown;
	SoundVolumeButton* sfxVolUp;
	SoundVolumeButton* sfxVolDown;

	SoundMuteButton* bgmmute;
	SoundMuteButton* sfxmute;
public:
	static SettingWindow* GetInstance();

	SettingWindow();
	~SettingWindow();
	void Update();
	void Init(int width, int height);
	void Render();
	void LeftClick(float, float);
	void LeftRelease(float, float);
	void UpdateMouseButton(glm::vec3);
};