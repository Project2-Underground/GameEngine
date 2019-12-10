#pragma once

#include <map>

#include "Button.h"
#include "Level.h"
#include "Player.h"

enum ScreenState {
	MENUSCREEN = 0,
	GAMESCREEN,
	CUTSCENE,
	ENDSCENE
};

class Screen {
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void RightClick(int, int) = 0;
	virtual void LeftClick(int, int) = 0;
	virtual int GetPointedObject(glm::vec3 pos) { return 1; };
};

class MenuScreen :public Screen {
	Button* play;
	Button* setting;
	Button* quit;
	UIObject* background;
	std::vector<UIObject*> UIs;
public:
	MenuScreen();
	void Render();
	void Update();
	void RightClick(int, int) {};
	void LeftClick(int, int);
	void UpdateMouseState(int x, int y);
};

class GameScreen :public Screen {
	vector<UIObject*> UI;
	vector<std::string> levels;
	Player* player;
	Level* currentLevel;
	Button* pause;
	// player
public:
	GameScreen(int level);
	void Render();
	void Update();
	void RightClick(int, int);
	void LeftClick(int, int);
	void ChangeLevel(int level);
	void ChangeRoom(std::string, std::string);

	int GetPointedObject(glm::vec3 pos);
	Player* GetPlayer() { return player; };
};

class CutsceneScreen :public Screen {
public:
	CutsceneScreen();
	void Render();
	void Update();
	void RightClick(int, int) {};
	void LeftClick(int, int);
};
