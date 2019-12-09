#pragma once

#include <vector>
#include "Player.h"
#include "DrawableObject.h"
#include "GLRenderer.h"
#include "Collider.h"
#include "Camera.h"
#include "Cursor.h"
#include "Button.h"
#include "LevelGenerator.h"
#include "Screen.h"

enum objectType {
	IMAGE_OBJ = 0,
	INTERACT_OBJ,
	ITEM,
	PORTAL,
	NPC,
};

using namespace std;
class Game
{
	static Game* instance;

	Screen* currentScreen;
	ScreenState currentState;

	GLRenderer *renderer;
	Camera* camera;
	Game();
	CursorUI* cursorGame;

	bool changeScreen;
public:
	int winWidth, winHeight;
	~Game();
	static Game* GetInstance();
	GLRenderer * GetRenderer();
	void Init(int width, int height);
	void Update();
	void Render();
	void UpdateScreenState();
	void ChangeScreenState(ScreenState newState);
	
	CursorUI* getCursor() { return cursorGame; };
};
