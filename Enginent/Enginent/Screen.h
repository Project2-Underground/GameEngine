#pragma once

#include <map>

#include "Button.h"
#include "Level.h"
#include "Player.h"

enum ScreenState {
	MENUSCREEN = 0,
	GAMESCREEN,
	CUTSCENE,
	PUZZLE
};

class Screen {
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void OnMouseDown() = 0;
	virtual void OnMouseUp() = 0;
	virtual void OnMouseDrag() = 0;
};

class MenuScreen :public Screen {
	Button* play;
	Button* setting;
	Button* quit;
public:
	MenuScreen();
	void Render();
	void Update();
	void OnMouseDown();
	void OnMouseUp();
	void OnMouseDrag();
};

class GameScreen :public Screen {
	vector<UIObject*> UI;
	Player* player;
	Level* currentLevel;
	Button* pause;
	// player
public:
	GameScreen(int level);
	void Render();
	void Update();
	void OnMouseDown();
	void OnMouseUp();
	void OnMouseDrag();
	void ChangeLevel();
};

class CutsceneScreen :public Screen {
public:
	CutsceneScreen();
	void Render();
	void Update();
	void OnMouseDown();
	void OnMouseUp() {};
	void OnMouseDrag() {};
};
