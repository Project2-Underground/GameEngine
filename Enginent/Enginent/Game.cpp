#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
//#include "GameObject.h"
#include "Item.h"
#include "ImageObject.h"
#include "TextObject.h"
#include "InteractObj.h"
#include "Door.h"


Game* Game::instance = nullptr;

Game * Game::GetInstance()
{
	if (instance == nullptr) {
		instance = new Game();
	}
	return instance;
}

GLRenderer * Game::GetRenderer()
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
	SoundManager::GetInstance()->Init();

	SquareMeshVbo * square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);

	TriangleMeshVbo * triangle = new TriangleMeshVbo();
	triangle->LoadData();
	renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

}

void Game::Update()
{
	currentScreen->Update();
}

void Game::Render()
{
	currentScreen->Render();
}

void Game::UpdateScreenState() {
	switch (currentState)
	{
	case MENUSCREEN:
		break;
	case GAMESCREEN:
		break;
	case CUTSCENE:
		break;
	case PUZZLE:
		break;
	default:
		break;
	}
}

void Game::ChangeScreenState(ScreenState newState) {
	currentState = newState;
	changeScreen = true;
}

Game::Game()
{
	currentState = MENUSCREEN;
	changeScreen = false;
	renderer = nullptr;
}

Game::~Game()
{
	delete camera;
	delete renderer;
}
