#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "InfoPhone.h"
#include "Item.h"
#include "ImageObject.h"
#include "TextObject.h"
#include "InteractObj.h"


Game* Game::instance = nullptr;

Game* Game::GetInstance()
{
	if (instance == nullptr) {
		instance = new Game();
	}
	return instance;
}

GLRenderer* Game::GetRenderer()
{
	return this->renderer;
}

void Game::Init(int width, int height)
{
	muteBG = false;
	muteSFX = false;
	winWidth = width;
	winHeight = height;
	renderer = new GLRenderer(width, height);
	renderer->InitGL("Shader/vertext.shd", "Shader/fragment.shd");
	renderer->SetOrthoProjection(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f);
	renderer->SetClearColor(1.0f, 1.0f, 200.0f / 255);

	SDL_ShowCursor(SDL_DISABLE);
	camera = Camera::GetInstance();
	SoundManager::GetInstance()->Init();

	SquareMeshVbo* square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);

	TriangleMeshVbo* triangle = new TriangleMeshVbo();
	triangle->LoadData();
	renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	currentState = TESTSCENE;
	UpdateScreenState();
	cursorGame = new CursorUI();
}

void Game::Update()
{
	if (changeScreen) {
		UpdateScreenState();
		changeScreen = false;
	}
	currentScreen->Update();
	cursorGame->updateCursor();
}

void Game::Render()
{
	currentScreen->Render();
	GetRenderer()->Render(cursorGame);
}

void Game::UpdateScreenState() {
	delete currentScreen;
	switch (currentState)
	{
	case MENUSCREEN:
		currentScreen = new MenuScreen();
		break;
	case GAMESCREEN:
		currentScreen = new GameScreen();
		break;
	case CUTSCENE:
		currentScreen = new CutsceneScreen();
		break;
	case ENDSCENE:
		break;
	case TESTSCENE:
		currentScreen = new TestScreen();
	default:
		break;
	}
}

//void Game::RightClick(int x, int y) {
//	currentScreen->RightClick(x, y);
//}
//
//void Game::LeftClick(int x, int y) {
//	currentScreen->LeftClick(x, y);
//}

void Game::HandleKey(SDL_Keycode key) {
	currentScreen->HandleKey(key);
}

//void Game::UpdateMouseState(int x, int y) {
//	if (currentState == MENUSCREEN) {
//		currentScreen->UpdateMouseState(x, y);
//	}
//}

void Game::ChangeScreenState(ScreenState newState) {
	currentState = newState;
	changeScreen = true;
}

int Game::GetScreenState()
{
	return currentState;
}

Game::Game()
{
	currentState = MENUSCREEN;
	changeScreen = false;
	renderer = nullptr;
}

Game::~Game()
{
	delete renderer;
	delete currentScreen;
	delete cursorGame;
}

glm::vec3 Game::FindMousePosition(int x, int y)
{
	float realX, realY;
	realX = -(winWidth * 0.5f) + x;
	realY = -(winHeight * 0.5f) + (winHeight - y);
	return glm::vec3(realX, realY, 1);
}

Level* Game::GetCurrentLevel() {
	return ((GameScreen*)currentScreen)->GetCurrentLevel();
}

Player* Game::GetPlayer() {
	return ((GameScreen*)currentScreen)->GetPlayer();
}