
#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "GameObject.h"
#include "CombineObject.h"
#include "ImageObject.h"

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
	renderer->SetOrthoProjection(-540, 540, -360, 360);
	renderer->SetClearColor(1.0f, 1.0f, 200.0f / 255);

	SquareMeshVbo * square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);

	TriangleMeshVbo * triangle = new TriangleMeshVbo();
	triangle->LoadData();
	renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	ImageObject * ice = new ImageObject();
	ice->SetTexture("Texture/IceBerge.png");
	ice->SetSize(600.0f, -600.0f);
	objects.push_back(ice);

	ImageObject * qq = new ImageObject();
	qq->SetTexture("Texture/qq.png");
	qq->SetSize(100.0f, -100.0f);
	qq->Translate(glm::vec3(2.0f, 0.5f, 0.0f));
	objects.push_back(qq);

	player = new Player();
	player->SetTexture("Texture/penguin.png");
	player->SetSize(100.0f, -100.0f);
	objects.push_back(player);

	Collider *col = new Collider(player);
	colliders.push_back(col);
	player->SetCollder(col);


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


