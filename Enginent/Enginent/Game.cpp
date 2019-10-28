
#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "GameObject.h"
#include "CombineObject.h"
#include "ImageObject.h"
#include "TextObject.h"

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
		cout << "x, y: " << realX << ", " << realY << endl;
		cout << "collide\n";
	}
}

void Game::HandleKey(char ch)
{
	if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(2);
		switch (ch) {
		case 'u': obj->Translate(glm::vec3(0, 0.3, 0)); break;
		case 'd': obj->Translate(glm::vec3(0, -0.3, 0)); break;
		case 'l': obj->Translate(glm::vec3(-0.3, 0, 0)); break;
		case 'r': obj->Translate(glm::vec3(0.3, 0, 0)); break;
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

	TriangleMeshVbo * triangle = new TriangleMeshVbo();
	triangle->LoadData();
	renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	ImageObject * _roomElias = new ImageObject();
	_roomElias->SetTexture("Texture/EliasRoom/room1.png");
	_roomElias->SetSize(width, -height);
	objects.push_back(_roomElias);

	player = new Player();
	player->SetTexture("Texture/Character/Elias.png");
	player->SetSize(100.0f, -240.0f);
	player->SetPosition(glm::vec3(0.0f, -80.0f, 1.0f));
	objects.push_back(player);

	SDL_Color a;
	a.r = 0;
	a.b = 0;
	a.g = 0;

	TextObject * text = new TextObject();
	text->loadText("Jin's fault", a, 100);
	text->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	objects.push_back(text);

	Collider *col = new Collider(player);
	colliders.push_back(col);
	player->SetCollder(col);

	player->anim->Play("Move", false);

	//CombineObject * obj = new CombineObject();
	//obj->Translate(glm::vec3(-1.0f, 1.0f, 0.0f));
	//obj->SetColor(1.0, 0.0, 0.0);
	//obj->SetColor2(0.0, 1.0, 0.0);
	//objects.push_back(obj);

	//GameObject * obj2 = new GameObject();
	//obj2->SetColor(0.0, 0.0, 1.0);
	//obj2->Translate(glm::vec3(1.0, 1.0, 0));
	//objects.push_back(obj2);

}

void Game::Update()
{
	player->Move();
	player->anim->Update();
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


