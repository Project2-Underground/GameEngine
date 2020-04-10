#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "InfoPhone.h"
#include "Item.h"
#include "UIObject.h"
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
	winWidth = width;
	winHeight = height;
	renderer = new GLRenderer(width, height);
	renderer->InitGL("Shader/vertext.shd", "Shader/fragment.shd");
	renderer->SetOrthoProjection(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f);
	renderer->SetClearColor(1.0f, 1.0f, 200.0f / 255);

	SDL_ShowCursor(SDL_DISABLE);
	camera = Camera::GetInstance();
	sound = SoundManager::GetInstance();
	script = ScriptManager::GetInstance();

	SquareMeshVbo* square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);

	TriangleMeshVbo* triangle = new TriangleMeshVbo();
	triangle->LoadData();
	renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	// test static loading image
	loadingImage = new UIObject();
	loadingImage->SetSize((float)width, -(float)height);
	loadingImage->SetTexture("Texture/tmp_texture/loading.png");

	currentState = MENUSCREEN;
	cursorGame = new CursorUI();
	UpdateScreenState();
}

void Game::Update()
{
	if (changeScreen && !isLoading) {
		changeScreen = false;
		UpdateScreenState();
	}
	else if (loadGame && !isLoading) {
		loadGame = false;
		SaveLoad(filename);
	}
	else if (loadLevel && !isLoading) {
		loadLevel = false;
		((GameScreen*)currentScreen)->ChangeLevel(nextLvl);
	}
	currentScreen->Update();
	cursorGame->updateCursor();
}

void Game::Render()
{
	currentScreen->Render();
	GetRenderer()->Render(cursorGame);
	if (isLoading) {
		if(!save)
			renderer->Render(loadingImage);
		isLoading = false;
	}
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
	/*case TESTSCENE:
		currentScreen = new TestScreen();*/
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
	changeScreen = true;
	isLoading = true;
	currentState = newState;
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
	XMLManager::GetInstance()->SaveGameOptions();

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


void Game::SaveLoad(std::string filename) {
	if (save) 
		SaveGame(filename);
	else {
		if (!dynamic_cast<GameScreen*>(currentScreen)) {
			currentState = GAMESCREEN;
			UpdateScreenState();
		}
		LoadGame(filename);
	}
}

void Game::SaveGame(std::string filename) {
	XMLManager::GetInstance()->SaveGame(filename);
}

void Game::LoadGame(std::string filename) {
	XMLManager::GetInstance()->LoadFromSave(filename);
	currentScreen->CloseGameAllWindow();
}