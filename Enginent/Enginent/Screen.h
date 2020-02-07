#pragma once

#include <map>

#include "Button.h"
#include "Level.h"
#include "Player.h"
#include "InfoPhone.h"
#include "TextBox.h"
#include "Inventory.h"
#include "GameViewWindow.h"

enum ScreenState {
	MENUSCREEN = 0,
	GAMESCREEN,
	CUTSCENE,
	ENDSCENE,
	EXIT,
	TESTSCENE
};

class Screen {
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void RightClick(glm::vec3, glm::vec3) = 0;
	virtual void LeftClick(glm::vec3, glm::vec3) = 0;
	virtual void HandleKey(SDL_Keycode) = 0;
	virtual void UpdateMouseState(glm::vec3 x, glm::vec3 y) = 0;
	virtual int GetPointedObject(glm::vec3 pos) { return 1; };
};

class MenuScreen :public Screen {
	Button* play;
	Button* load;
	Button* setting;
	Button* quit;
	UIObject* background;
	std::vector<UIObject*> UI;
public:
	MenuScreen();
	void Render();
	void Update();
	void RightClick(glm::vec3, glm::vec3);
	void LeftClick(glm::vec3, glm::vec3);
	void HandleKey(SDL_Keycode);
	void UpdateMouseState(glm::vec3, glm::vec3);
	~MenuScreen();
};

class GameScreen :public Screen {
	vector<UIObject*> UI;

	Player* player;
	Phone* phone;

	Button* pause;
	Level* currentLevel;
	TextBox* dialogueText;

	Inventory* inventory;
	ViewWindow* viewWin;

public:
	vector<std::string> levels;

	GameScreen();
	void Render();
	void Update();
	void RightClick(glm::vec3, glm::vec3);
	void LeftClick(glm::vec3, glm::vec3);
	void HandleKey(SDL_Keycode);
	void UpdateMouseState(glm::vec3, glm::vec3);
	void ChangeLevel(int level);
	void ChangeRoom(std::string, std::string);
	void LoadGame(std::string);

	Level* GetCurrentLevel() { return currentLevel; }
	int GetPointedObject(glm::vec3 pos);
	Player* GetPlayer() { return player; };
	Inventory* GetInventory() { return inventory; };

	~GameScreen();
};

class CutsceneScreen :public Screen {
public:
	CutsceneScreen();
	void Render();
	void Update();
	void RightClick(glm::vec3, glm::vec3) {};
	void LeftClick(glm::vec3, glm::vec3);
	void UpdateMouseState(glm::vec3, glm::vec3);
	void HandleKey(SDL_Keycode);
};

class TestScreen : public Screen {
public:
	std::vector<UIObject*> UI;
	UIObject* background;
	Puzzle* puzzle;
public:
	TestScreen();
	void Render();
	void Update();
	void RightClick(glm::vec3, glm::vec3);
	void LeftClick(glm::vec3, glm::vec3);
	void HandleKey(SDL_Keycode);
	void UpdateMouseState(glm::vec3, glm::vec3);
	~TestScreen();
};
