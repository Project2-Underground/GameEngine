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
	bool splashScreen;
	float splashScreenTime;
	Button* play;
	Button* load;
	Button* setting;
	Button* quit;
	UIObject* background;
	UIObject* SplashScreen;
	UIObject* Credit;
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

	UIObject* cutscene;

	bool PuzzleTime;
	bool InventoryEnable;
public:
	vector<std::string> levels;
	std::vector<Item*> items;
	vector<std::string> objActions;
	map<std::string, Puzzle*> puzzles;
	map<std::string, unsigned int> cutscenes;
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
	void OpenPuzzle(std::string);
	void ResetPuzzle();
	void ClosePuzzle();
	bool IsPuzzleOpen() { return PuzzleTime; }

	Door* GetDoor(std::string);
	Level* GetCurrentLevel() { return currentLevel; }
	InteractTypeList GetPointedObject(glm::vec3 pos);
	Player* GetPlayer() { return player; };
	Inventory* GetInventory() { return inventory; };
	Item* FindItem(std::string name);
	Puzzle* FindPuzzle(std::string name);

	~GameScreen();

	// special npcs
	Butler* butler;
};
