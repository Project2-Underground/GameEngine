
#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "GameObject.h"
#include "CombineObject.h"
#include "TextObject.h"
#include "InteractObj.h"

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

void Game::rightClick(int x, int y)
{
	float realX, realY;
	realX = -(winWidth * 0.5) + x;
	realY = -(winHeight * 0.5) + (winHeight - y);
	player->setTarget(realX, realY);

}

void Game::leftClick(int x, int y)
{
	float realX, realY;
	realX = -(winWidth * 0.5) + x;
	realY = -(winHeight * 0.5) + (winHeight - y);
	if (colliders[0]->isClicked(realX, realY)) {
		cout << "collide\n";
	}
}

void Game::HandleKey(char ch)
{
	if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(2);
		switch (ch) {
		case 'u': objects.push_back(player->setDialogue("Fly")); break;
		case 'd': break;
		case 'l': break;
		case 'r': break;
		}
	}
}

void Game::Init(int width, int height)
{
	winWidth = width;
	winHeight = height;
	renderer = new GLRenderer(width, height);
	renderer->InitGL("Shader/vertext.shd", "Shader/fragment.shd");
	renderer->SetOrthoProjection(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f);
	renderer->SetClearColor(1.0f, 1.0f, 200.0f / 255);

	SquareMeshVbo * square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);

	//TriangleMeshVbo * triangle = new TriangleMeshVbo();
	//triangle->LoadData();
	//renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	ImageObject * _roomElias = new ImageObject();
	_roomElias->SetTexture("Texture/EliasRoom/room1.png");
	_roomElias->SetSize(width, -height);
	objects.push_back(_roomElias);

	ImageObject * _door = new ImageObject();
	_door->SetTexture("Texture/EliasRoom/door.png");
	_door->SetSize(150, -300);
	_door->SetPosition(glm::vec3(200.0f, 0.0f, 1.0f));
	objects.push_back(_door);

	//Collider *col2 = new Collider(_door);
	//colliders.push_back(col2);
	//player->SetCollder(col2);

	player = new Player();
	player->SetTexture("Texture/Character/Elias.png");
	player->SetSize(100.0f, -240.0f);
	player->SetPosition(glm::vec3(0.0f, -80.0f, 1.0f));
	objects.push_back(player);

	objects.push_back(player->setDialogue("What?"));

	Collider *col = new Collider(player);
	colliders.push_back(col);
	player->SetCollder(col);
}

void Game::Update()
{
	player->Move();
}

void Game::Render()
{
	this->GetRenderer()->Render(this->objects);
}

Game::Game()
{
	for (DrawableObject* obj : objects) {
		delete obj;
	}
	renderer = nullptr;
}


