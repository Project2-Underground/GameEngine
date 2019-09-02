
#include "Game.h"
#include "GLRenderer.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "GameObject.h"

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

void Game::HandleMouse(int x, int y)
{
	float realX, realY;
	realX = -3 + x * (6.0 / winWidth);
	realY = -3 + (winHeight - y) * (6.0 / winHeight);
	if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(0);
		obj->SetPosition(glm::vec3(realX, realY, 0));
	}
}

void Game::HandleKey(char ch)
{
	if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(0);
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

	/*SquareMeshVbo * square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);*/
	
	TriangleMeshVbo * triangle = new TriangleMeshVbo();
	triangle->LoadData();
	renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	GameObject * obj = new GameObject();
	obj->SetColor(1.0, 0.0, 0.0);
	objects.push_back(obj);

	GameObject * obj2 = new GameObject();
	obj2->SetColor(0.0, 1.0, 0.0);
	obj2->Translate(glm::vec3(1.0, 1.0, 0));
	objects.push_back(obj2);

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


