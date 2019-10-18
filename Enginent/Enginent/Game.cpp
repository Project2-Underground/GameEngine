
#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "GameObject.h"
#include "CombineObject.h"
#include "TextObject.h"
#include "InteractObj.h"

Game* Game::instance = nullptr;

enum objectType {
	IMAGE_OBJ = 0,
	INTERACT_OBJ,
	PORTOL,
	NPC
};


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
	for (int i = 0; i < colliders.size(); i++)
	{
		if (colliders[i]->isClicked(realX, realY)) {
			cout << "collide\n";
			printf("%d\n", i);
		}
	}

}

void Game::HandleKey(char ch)
{
	if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(2);
		switch (ch) {
		case 'u': player->setDialogue("Fly"); break;
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

	//
	//create object
	//
	createObject(IMAGE_OBJ, "Texture/EliasRoom/room1.png", width, -height, glm::vec3(0.0f,0.0f,1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/door.png", 220, -350, glm::vec3(480.0f,30.0f,1.0f), "Lock");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Bed.png", 450, -280, glm::vec3(-415.0f, -100.0f, 1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/cloth.png", 300, -150, glm::vec3(-305.0f, -140.0f, 1.0f), "Guess it is time to do the laundry.");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/TV.png", 300, -250, glm::vec3(0.0f, -30.0f, 1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Hoody.png", 150, -300, glm::vec3(250.0f, -5.0f, 1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Poster1.png", 150, -200, glm::vec3(-430.0f, 100.0f, 1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Poster2.png", 150, -150, glm::vec3(-240.0f, 100.0f, 1.0f), "");


	//
	//Create player
	//
	player = new Player();
	player->SetTexture("Texture/Character/Elias.png");
	player->SetSize(150.0f, -340.0f);
	player->SetPosition(glm::vec3(0.0f, -50.0f, 1.0f));
	objects.push_back(player);

	objects.push_back(player->setDialogue(" "));

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

void Game::createObject(int type, std::string texture, int sizeX, int sizeY, glm::vec3 pos, std::string dialogue)
{
	ImageObject *tmp = nullptr;
	switch (type)
	{
		case IMAGE_OBJ:
		{
			tmp = new ImageObject();
			break;
		}
		case INTERACT_OBJ:
		{
			if(dialogue != "")
			{
				tmp = new InteractableObj(dialogue);
				Collider *col2 = new Collider(tmp);
				colliders.push_back(col2);
				((InteractableObj*)tmp)->SetCollder(col2);

			}
			else
			{
				tmp = new InteractableObj();
				Collider *col2 = new Collider(tmp);
				colliders.push_back(col2);
				((InteractableObj*)tmp)->SetCollder(col2);
			}
			break;
		}
		case PORTOL:
		{
			break;
		}
		case NPC:
		{
			break;
		}
	}

	tmp->SetTexture(texture);
	tmp->SetSize(sizeX, sizeY);
	tmp->SetPosition(pos);

	objects.push_back(tmp);
}

