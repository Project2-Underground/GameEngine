#pragma once

#include <map>

#include "Button.h"
#include "Level.h"
#include "Player.h"
#include "InfoPhone.h"

enum ScreenState {
	MENUSCREEN = 0,
	GAMESCREEN,
	CUTSCENE,
	ENDSCENE,
	EXIT
};

class Screen {
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void RightClick(int, int) = 0;
	virtual void LeftClick(int, int) = 0;
	virtual void HandleKey(SDL_Keycode) = 0;
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
	void RightClick(int, int) {};
	void LeftClick(int, int);
	void HandleKey(SDL_Keycode);
	void UpdateMouseState(int x, int y);

	~MenuScreen();
};

class GameScreen :public Screen {
	vector<UIObject*> UI;

	Player* player;
	Phone* phone;

	Button* pause;
	Level* currentLevel;
public:
	vector<std::string> levels;

	GameScreen();
	void Render();
	void Update();
	void RightClick(int, int);
	void LeftClick(int, int);
	void HandleKey(SDL_Keycode);
	void ChangeLevel(int level);
	void ChangeRoom(std::string, std::string);
	void LoadGame(std::string);

	Level* GetCurrentLevel() { return currentLevel; }
	int GetPointedObject(glm::vec3 pos);
	Player* GetPlayer() { return player; };

	~GameScreen();
};

class CutsceneScreen :public Screen {
public:
	CutsceneScreen();
	void Render();
	void Update();
	void RightClick(int, int) {};
	void LeftClick(int, int);
	void HandleKey(SDL_Keycode);
};
