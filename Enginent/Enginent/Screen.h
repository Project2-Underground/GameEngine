#pragma once

#include <map>

#include "Button.h"
#include "Level.h"

enum objectType {
	IMAGE_OBJ = 0,
	INTERACT_OBJ,
	ITEM,
	PORTAL,
	NPC,
};

enum ScreenType {
	MENUSCREEN = 0,
	GAMESCREEN,
	CUTSCENE,
	PUZZLE
};

class Screen {
public:
	virtual void OnRender() = 0;
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
	void OnRender();
	void OnMouseDown();
	void OnMouseUp();
	void OnMouseDrag();
};

class GameScreen :public Screen {
	Level* currentLevel;
	Button* pause;
	// player
public:
	GameScreen(int level);
	void OnRender();
	void OnMouseDown();
	void OnMouseUp();
	void OnMouseDrag();
	void ChangeLevel();
};

class CutsceneScreen :public Screen {
public:
	CutsceneScreen();
	void OnRender();
	void OnMouseDown();
	void OnMouseUp() {};
	void OnMouseDrag() {};
};
