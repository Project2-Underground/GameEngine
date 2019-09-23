
#include "Game.h"
#include "GLRenderer.h"
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
	realX = -3 + x * (6.0 / winWidth);
	realY = -3 + (winHeight - y) * (6.0 / winHeight);
	if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(2);
		obj->SetPosition(glm::vec3(realX, realY, 0));
	}
}

void Game::leftClick(int x, int y)
{

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
	renderer->SetOrthoProjection(-3, 3, -3, 3);
	renderer->SetClearColor(1.0f, 1.0f, 200.0f / 255);

	SquareMeshVbo * square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);

	TriangleMeshVbo * triangle = new TriangleMeshVbo();
	triangle->LoadData();
	renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	ImageObject * ice = new ImageObject();
	ice->SetTexture("Texture/IceBerge.png");
	ice->SetSize(6.0f, -6.0f);
	objects.push_back(ice);

	ImageObject * qq = new ImageObject();
	qq->SetTexture("Texture/qq.png");
	qq->SetSize(1.0f, -1.0f);
	qq->Translate(glm::vec3(2.0f, 0.5f, 0.0f));
	objects.push_back(qq);

	ImageObject * penguin = new ImageObject();
	penguin->SetTexture("Texture/penguin.png");
	penguin->SetSize(1.0f, -1.0f);
	objects.push_back(penguin);





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


