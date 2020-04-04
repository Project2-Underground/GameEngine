#pragma once

#include <vector>
#include <thread>

#include "Player.h"
#include "DrawableObject.h"
#include "GLRenderer.h"
#include "Collider.h"
#include "Camera.h"
#include "Cursor.h"
#include "Button.h"
#include "XMLManager.h"
#include "Screen.h"
#include "TextBox.h"
#include "ScriptManager.h"

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
	ScriptManager* script;
	SoundManager* sound;
	LoadingScreen* loadingScreen;

	bool quit = false;
	bool changeScreen;
	bool save;

	void SaveGame(std::string);
	void LoadGame(std::string);

	std::thread loadingThread;
public:
	int winWidth, winHeight;
	~Game();
	static Game* GetInstance();
	GLRenderer* GetRenderer();
	Screen* GetScreen() { return currentScreen; };
	Camera* GetCamera() { return camera; };
	CursorUI* GetCursor() { return cursorGame; }
	void Init(int width, int height);
	void Update();
	void Render();
	void UpdateScreenState();
	void ChangeScreenState(ScreenState newState);
	int GetScreenState();
	//void RightClick(int, int);
	//void LeftClick(int, int);
	void HandleKey(SDL_Keycode);
	//void UpdateMouseState(int, int);
	void quitGame() { quit = true; };
	bool getQuitState() { return quit; };

	void SetSaveGame(bool b) { save = b; }
	void SaveLoad(std::string);

	Level* GetCurrentLevel();
	Player* GetPlayer();
	glm::vec3 FindMousePosition(int, int);

	bool isLoading;
};
