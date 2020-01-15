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

	GLRenderer* renderer;
	Camera* camera;
	Game();
	CursorUI* cursorGame;
	bool quit = false;
	bool changeScreen;

public:
	int winWidth, winHeight;
	~Game();
	static Game* GetInstance();
	GLRenderer* GetRenderer();
	Screen* GetScreen() { return currentScreen; };
	Camera* GetCamera() { return camera; };
	void Init(int width, int height);
	void Update();
	void Render();
	void UpdateScreenState();
	void ChangeScreenState(ScreenState newState);
	int GetScreenState();
	void RightClick(int, int);
	void LeftClick(int, int);
	void UpdateMouseState(int, int);
<<<<<<< HEAD
	void quitGame() { quit = true; };
	bool getQuitState() { return quit; };
	
=======

	Level* GetCurrentLevel();
	Player* GetPlayer();
>>>>>>> ed0e37d6e7c7def4780cb7ade54502a82fe904e8
	glm::vec3 FindMousePosition(int, int);
	CursorUI* getCursor() { return cursorGame; };
};
