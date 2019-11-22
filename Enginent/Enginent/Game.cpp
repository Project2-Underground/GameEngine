
#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "GameObject.h"
#include "CombineObject.h"
#include "TextObject.h"
#include "InteractObj.h"
#include "LevelGenerator.h"
#include "Cursor.h"
#include "Button.h"

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

int Game::checkPointObject(glm::vec3 pos)
{
	for (int i = objects.size() - 1; i >= 0; i--)
	{
		if (dynamic_cast<InteractableObj*>(objects[i]) && dynamic_cast<InteractableObj*>(objects[i])->checkPointing(pos.x, pos.y))
		{
			return dynamic_cast<InteractableObj*>(objects[i])->getType();
		}
	}

	return 0;
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
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<InteractableObj*>(objects[i]))
		{
			((InteractableObj*)objects[i])->checkCollider(realX, realY);
		}
	}

	for (int j = 0; j < UI.size(); j++)
	{
		if (dynamic_cast<Button*>(UI[j]))
		{
			((Button*)UI[j])->checkCollider(realX, realY);
		}
	}
}

void Game::updateMouseState(int x, int y)
{
	glm::vec3 realPos = Game::GetInstance()->findRealPos(x, y);
	for (int i = 0; i < UI.size(); i++)
	{
		if (dynamic_cast<Button*>(UI[i]))
		{
			((Button*)UI[i])->updateButton(realPos.x, realPos.y);
		}
	}
}

void Game::HandleKey(char ch)
{
	if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(2);
		switch (ch) {
		case 'u': player->setDialogue("Up"); break;
		case 'd': player->setDialogue("Down"); break;
		case 'l': player->setDialogue("Left"); break;
		case 'r': player->setDialogue("Right"); break;
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

	SDL_ShowCursor(SDL_DISABLE);

	SquareMeshVbo * square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);

	//TriangleMeshVbo * triangle = new TriangleMeshVbo();
	//triangle->LoadData();
	//renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	//
	//create object
	//
	/*createObject(IMAGE_OBJ, "Texture/EliasRoom/room1.png", width, -height, glm::vec3(0.0f,0.0f,1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/door.png", 220, -350, glm::vec3(480.0f,30.0f,1.0f), "Lock");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Bed.png", 450, -280, glm::vec3(-415.0f, -100.0f, 1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/cloth.png", 300, -150, glm::vec3(-305.0f, -140.0f, 1.0f), "Guess it is time to do the laundry.");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/TV.png", 300, -250, glm::vec3(0.0f, -30.0f, 1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Hoody.png", 150, -300, glm::vec3(250.0f, -5.0f, 1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Poster1.png", 150, -200, glm::vec3(-430.0f, 100.0f, 1.0f), "");
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Poster2.png", 150, -150, glm::vec3(-240.0f, 100.0f, 1.0f), "");
*/
	std::string filename("example_xml_file_format/map.xml");
	RoomGenerator room;
	room.GenerateRoom(filename);

	//
	//Create player
	//
	player = new Player();
	player->SetTexture("Texture/Character/Elias.png");
	player->SetSize(230.0f, -350.0f);
	player->SetPosition(glm::vec3(0.0f, -50.0f, 1.0f));
	player->setDisplay(false);
	objects.push_back(player);

	Collider *col = new Collider(player);
	colliders.push_back(col);
	player->SetCollder(col);

	player->anim->Play("Move", true);

	objects.push_back(player->createDialogueText());

	//Item* item1 = new Item("temp");
	//item1->SetTexture("Texture/EliasRoom/cloth.png");

	//player->inventory->addItem(item1);
	//player->inventory->addItem(item1);
	//player->inventory->addItem(item1);
	//player->inventory->addItem(item1);
	//player->inventory->addItem(item1);



}

void Game::Update()
{
	player->Update();
	player->anim->Update();
	cursorGame->updateCursor();
}

void Game::Render()
{
	this->GetRenderer()->Render(this->objects);
	this->GetRenderer()->Render(this->UI);
}

Game::Game()
{
	for (DrawableObject* obj : objects) {
		delete obj;
	}
	renderer = nullptr;
}

void Game::createObject(int type, std::string texture, int sizeX, int sizeY, glm::vec3 pos, IneractTypeList objType, vector<std::string>* dialogue)
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
		if (dialogue != nullptr)
		{
			tmp = new InteractableObj(objType, dialogue);

		}
		else
		{
			tmp = new InteractableObj();
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
	if (type != IMAGE_OBJ)
	{
		Collider *col2 = new Collider(tmp);
		colliders.push_back(col2);
		((InteractableObj*)tmp)->SetCollder(col2);
	}

	objects.push_back(tmp);
}

void Game::AddObject(DrawableObject* obj) {
	objects.push_back(obj);
}

void Game::AddUI(UIObject* obj) {
	UI.push_back(obj);
}

Game::~Game()
{

}

glm::vec3 Game::findRealPos(int x, int y)
{
	float realX, realY;
	realX = -(winWidth * 0.5) + x;
	realY = -(winHeight * 0.5) + (winHeight - y);
	return glm::vec3(realX, realY, 1);
}

