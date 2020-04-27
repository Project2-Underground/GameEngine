#pragma once

#include <map>

#include "Button.h"
#include "Level.h"
#include "Player.h"
#include "InfoPhone.h"
#include "TextBox.h"
#include "Inventory.h"
#include "GameWindows.h"
#include "Door.h"
#include "SpecialNPCs.h"

enum ScreenState {
	MENUSCREEN = 0,
	GAMESCREEN,
	CUTSCENE,
	ENDSCENE,
	EXIT,
	TESTSCENE
};

class Screen {
protected:
	std::vector<GameWindow*> windows;
public:
	bool buttonClicked;
	bool Pause;
	virtual void Render() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void RightClick(glm::vec3, glm::vec3) = 0;
	virtual void LeftClick(glm::vec3, glm::vec3) = 0;
	virtual void RightRelease(glm::vec3, glm::vec3) = 0;
	virtual void LeftRelease(glm::vec3, glm::vec3) = 0;
	virtual void HandleKey(SDL_Keycode) = 0;
	virtual void UpdateMouseState(glm::vec3 x, glm::vec3 y) = 0;
	virtual void Scroll(glm::vec3, int) {};
	virtual InteractTypeList GetPointedObject(glm::vec3 pos) { return NORMAL; std::cout << "menu\n"; };
	bool GameWindowOpen();
	void CloseGameAllWindow();
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
	void Init() {}
	void Render();
	void Update();
	void RightClick(glm::vec3, glm::vec3);
	void LeftClick(glm::vec3, glm::vec3);
	void RightRelease(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void HandleKey(SDL_Keycode);
	void UpdateMouseState(glm::vec3, glm::vec3);
	~MenuScreen();
};

class GameScreen :public Screen {
	vector<UIObject*> UI;

	Player* player;
	Phone* phone;

	Button* pause;
	TextBox* dialogueText;

	Level* currentLevel;
	Puzzle* currentPuzzle;

	Inventory* inventory;

	bool PuzzleTime;
	bool InventoryEnable;
public:
	vector<std::string> levels;
	vector<std::string> objActions;
	map<std::string, Puzzle*> puzzles;
	PhoneOpenButton* phoneIcon;

	GameScreen();
	void Init();
	void Render();
	void Update();
	void RightClick(glm::vec3, glm::vec3);
	void LeftClick(glm::vec3, glm::vec3);
	void RightRelease(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void Scroll(glm::vec3, int);
	void HandleKey(SDL_Keycode);
	void UpdateMouseState(glm::vec3, glm::vec3);
	void ChangeLevel(int level);
	void ChangeRoom(std::string, std::string);
	void LoadGame(std::string);
	void OpenPuzzle(std::string);
	void ResetPuzzle();
	void ClosePuzzle();

	Door* GetDoor(std::string);
	Level* GetCurrentLevel() { return currentLevel; }
	InteractTypeList GetPointedObject(glm::vec3 pos);
	Player* GetPlayer() { return player; };
	Inventory* GetInventory() { return inventory; };
	Item* FindItem(std::string name);

	std::vector<Item*> items;

	~GameScreen();

	// special npcs
	Butler* butler;
};

// may change to other thing
class CutsceneScreen :public Screen {
public:
	CutsceneScreen();
	void Render();
	void Init() {}
	void Update();
	void RightClick(glm::vec3, glm::vec3);
	void LeftClick(glm::vec3, glm::vec3);
	void RightRelease(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void UpdateMouseState(glm::vec3, glm::vec3);
	void HandleKey(SDL_Keycode);
};

class TestScreen : public Screen {
public:
	std::vector<UIObject*> UI;
	UIObject* background;
	PuzzleTemplate* puzzle;
	Puzzle* numpad;
public:
	TestScreen();
	void Init();
	void Render();
	void Update();
	void RightClick(glm::vec3, glm::vec3);
	void LeftClick(glm::vec3, glm::vec3);
	void UpdateMouseState(glm::vec3, glm::vec3);
	void RightRelease(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3 screen, glm::vec3 world);
	void HandleKey(SDL_Keycode);
	~TestScreen();
};
