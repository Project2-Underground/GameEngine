#include "Game.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "GameObject.h"
#include "CombineObject.h"
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
	realX = -(winWidth * 0.5) + x - camera->GetPosition().x;
	realY = -(winHeight * 0.5) + (winHeight - y) - camera->GetPosition().y;
	player->SetNextPosition(realX, realY);
}

void Game::leftClick(int x, int y)
{
	float realX, realY;
	realX = -(winWidth * 0.5) + x - camera->GetPosition().x;
	realY = -(winHeight * 0.5) + (winHeight - y) - camera->GetPosition().y;
	for (int i = 0; i < objects.size(); i++)
	{
		if (InteractableObj * ib = dynamic_cast<InteractableObj*>(objects[i]))
		{
			if (ib->checkCollider(realX, realY))
				player->SetTarget(ib);
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
	glm::vec3 realPos = Game::GetInstance()->FindMousePosition(x, y);
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
	camera = Camera::GetInstance();
	SoundManager::GetInstance()->Init();
	SoundManager::GetInstance()->playSound("MainScreen");
	SquareMeshVbo * square = new SquareMeshVbo();
	square->LoadData();
	renderer->AddMesh(SquareMeshVbo::MESH_NAME, square);

	TriangleMeshVbo * triangle = new TriangleMeshVbo();
	triangle->LoadData();
	renderer->AddMesh(TriangleMeshVbo::MESH_NAME, triangle);

	vector<std::string> doorDialogue;
	doorDialogue.push_back("Lock.");
	doorDialogue.push_back("Seem like it needs card to unlock.");
	doorDialogue.push_back("I need to find a key card.");

	/*********************************************************************************************************************************************/
	/*********************************************************************************************************************************************/
	/*********************************************************************************************************************************************/



	/*********************************************************************************************************************************************/
	/********************************************************************Room 1*******************************************************************/
	/*********************************************************************************************************************************************/

	/*vector<std::string>* doorDialogue = new vector<std::string>;
	doorDialogue->push_back("Lock.");
	doorDialogue->push_back("Seem like it needs card to unlock.");
	doorDialogue->push_back("I need to find a key card.");

	createObject(IMAGE_OBJ, "Texture/EliasRoom/room1.png", width, -height, glm::vec3(0.0f, 0.0f, 1.0f), NORMAL, nullptr);
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias_Room_DoorAni.png", 208, -379, glm::vec3(420.0f, 20.0f, 1.0f), PICKUP, doorDialogue);
	createObject(INTERACT_OBJ, "Texture/EliasRoom/TV.png", 300, -250, glm::vec3(50.0f, -30.0f, 1.0f), NORMAL, nullptr);
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias_Room_Bed.png", 450, -250, glm::vec3(-330.0f, -120.0f, 1.0f), NORMAL, nullptr);
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Hoody.png", 150, -300, glm::vec3(250.0f, -5.0f, 1.0f), NORMAL, nullptr);
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Poster1.png", 150, -200, glm::vec3(-430.0f, 100.0f, 1.0f), NORMAL, nullptr);
	createObject(INTERACT_OBJ, "Texture/EliasRoom/Elias Room_Poster2.png", 150, -150, glm::vec3(-240.0f, 100.0f, 1.0f), NORMAL, nullptr);

	UIObject* border = new UIObject();
	border->SetTexture("Texture/UI/Black_Border.png");
	border->SetSize(1280, 720);
	border->SetPosition(glm::vec3(0, 0, 1));
	UI.push_back(border);
	*/

	UIObject* blackBold1 = new UIObject();
	blackBold1->SetTexture("Texture/UI/Black_Border.png");
	blackBold1->SetSize(1280, -720);
	blackBold1->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	UI.push_back(blackBold1);

	// testing item pickup

	std::string filename("example_xml_file_format/map.xml");
	RoomGenerator room;
	room.GenerateRoom(filename);
	// testing door -----------------------------------------
	Collider* door_next_limit = new Collider();
	door_next_limit->setNewSize(winWidth, winHeight);
	door_next_limit->setNewPos(winWidth, 0);

	// testing item pickup
	Item* item = new Item("Hoodies");
	item->SetTexture("Texture/EliasRoom/Elias Room_Hoody.png");
	item->SetInventoryTexture("Texture/EliasRoom/E Room_Hoody_I.png");
	item->SetSize(150, -300);
	item->SetPosition(glm::vec3(250.0f, -5.0f, 1.0f));
	item->SetCollder(new Collider(item));
	objects.push_back(item);

	Door* door = new Door(winWidth*0.5, -80, door_next_limit, item);
	door->SetTexture("Texture/EliasRoom/Elias_Room_Door.png");
	door->SetPosition(glm::vec3(420.0f, 20.0f, 1.0f));
	door->SetSize(208, -379);
	door->SetDialogue(doorDialogue);
	door->SetCollder(new Collider(door));						// collider of the door
	objects.push_back(door);
	// testing door -----------------------------------------

	player = new Player();
	player->SetTexture("Texture/Character/Elias_idle.png");
	player->SetSize(230.0f, -350.0f);
	player->SetPosition(glm::vec3(0.0f, -50.0f, 1.0f));
	player->setDisplay(false);
	objects.push_back(player);

	Collider *col = new Collider(player);
	colliders.push_back(col);
	player->SetCollder(col);
	
	col = new Collider();
	col->setNewWidth(winWidth * 0.75);
	col->setNewPos(winWidth * 0.25, 0);
	player->SetWalkLimit(col);


	player->anim->Play("Move", true);

	Camera::GetInstance()->SetTarget(player);

	Collider* limit = new Collider();
	limit->setNewSize(winWidth, winHeight);

	Camera::GetInstance()->SetLimit(limit);

	objects.push_back(player->createDialogueText());

	//CombineObject * obj = new CombineObject();
	//obj->Translate(glm::vec3(-1.0f, 1.0f, 0.0f));
	//obj->SetColor(1.0, 0.0, 0.0);
	//obj->SetColor2(0.0, 1.0, 0.0);
	//objects.push_back(obj);

	//GameObject * obj2 = new GameObject();
	//obj2->SetColor(0.0, 0.0, 1.0);
	//obj2->Translate(glm::vec3(1.0, 1.0, 0));
	//objects.push_back(obj2);

	/*********************************************************************************************************************************************/
	/**************************************************************Main screen********************************************************************/
	/*********************************************************************************************************************************************/
	//createObject(IMAGE_OBJ, "Texture/UI/MainScreen/MainScreen_Click.png", 1280, -720, glm::vec3(0.0f, 0.0f, 1.0f), NORMAL, nullptr);



	UIObject* mainScreen = new UIObject();
	mainScreen->SetTexture("Texture/UI/MainScreen/MainScreen_Click.png");
	mainScreen->SetSize(1280, -720);
	mainScreen->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	UI.push_back(mainScreen);

	Exit_Button* exitButton = new Exit_Button("Texture/UI/MainScreen/ExitBotton_Normal.png", "Texture/UI/MainScreen/ExitBotton_Point.png", "Texture/UI/MainScreen/ExitBotton_Click.png");
	exitButton->SetSize(300, -120);
	exitButton->SetPosition(glm::vec3(-170.0f, -50.0f, 1.0f));
	Collider *col3 = new Collider(exitButton);
	colliders.push_back(col3);
	((ImageObject*)exitButton)->SetCollder(col3);
	UI.push_back(exitButton);

	vector<ImageObject*> tmp;
	tmp.push_back(mainScreen);
	tmp.push_back(exitButton);

	SwitchScene_Button* startButton = new SwitchScene_Button("Texture/UI/MainScreen/StartBotton_Normal.png", "Texture/UI/MainScreen/StartBotton_Point.png", "Texture/UI/MainScreen/StartBotton_Click.png", tmp);
	startButton->SetSize(300, -120);
	startButton->SetPosition(glm::vec3(-170.0f, 70.0f, 1.0f));
	Collider *col2 = new Collider(startButton);
	colliders.push_back(col2);
	((ImageObject*)startButton)->SetCollder(col2);
	UI.push_back(startButton);

	//the last

	cursorGame = new CursorUI();
	UI.push_back(cursorGame);
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

void Game::createObject(int type, std::string texture, int sizeX, int sizeY, glm::vec3 pos, IneractTypeList objType, std::vector<std::string> dialogue)
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
		if (dialogue.size() > 0)
		{
			tmp = new InteractableObj(objType, dialogue);

		}
		else
		{
			tmp = new InteractableObj();
		}
		break;
	}
	case PORTAL:
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
	delete player;
	for (auto obj : objects)
		if (obj)
			delete obj;
	delete camera;
	delete renderer;
}


glm::vec3 Game::FindMousePosition(int x, int y)
{
	float realX, realY;
	realX = -(winWidth * 0.5) + x;
	realY = -(winHeight * 0.5) + (winHeight - y);
	return glm::vec3(realX, realY, 1);
}