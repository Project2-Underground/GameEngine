#include <algorithm>

#include "Level.h"
#include "XMLManager.h"
#include "Game.h"

void Room::Render() {
	// render objects
	Game::GetInstance()->GetRenderer()->Render(objects, true);
}

Room::~Room() {
	delete playerWalkLimit;
	delete cameraLimit;
	for (auto obj : objects)
		delete obj;
}


void Room::Update() {

}

void Room::SetPlayerWalkLimit(Collider* col) {
	delete playerWalkLimit;
	playerWalkLimit = col;
}

void Room::SetCameraLimit(Collider* col) {
	delete cameraLimit;
	cameraLimit = col;
}

void Room::RightClick(float x, float y) {
	Game* game = Game::GetInstance();
	((GameScreen*)game->GetScreen())->GetPlayer()->SetNextPosition((float)x, (float)y);
}

void Room::LeftClick(float x, float y) {
	Game* game = Game::GetInstance();

	for (int i = 0; i < objects.size(); i++)
		if (InteractableObj * ib = dynamic_cast<InteractableObj*>(objects[i]))
			if (ib->CheckCollider((float)x, (float)y))
				((GameScreen*)game->GetScreen())->GetPlayer()->CheckTarget(ib);
}

void Room::SortObjLayer() {
	// sort the main layers
	DrawableObject* key;
	int ptr;
	for (int i = 1; i < objects.size(); i++) {
		key = objects[i]; 
		ptr = i - 1;
		while (key->layer < objects[ptr]->layer && ptr >= 0) {
			objects[ptr + 1] = objects[ptr];
			ptr--;
		}
		objects[ptr + 1] = key;
	}

	// sort the layers within each main layers
	int iStart = 0;
	Layers currentLayer = BACKGROUND_LAYER;
	for (int i = 0; i < objects.size(); i++) {
		if ((Layers)objects[i]->layer != currentLayer || i == objects.size() - 1) {
			for (int j = iStart + 1; j <= i; j++) {
				key = objects[j];
				ptr = j - 1;
				while (key->subLayer < objects[ptr]->subLayer && ptr >= iStart) {
					objects[ptr + 1] = objects[ptr];
					ptr--;
				}
				objects[ptr + 1] = key;
			}
			currentLayer = (Layers)objects[i]->layer;
			iStart = i;
		}
	}
/*
	std::cout << "--------------------------------layers--------------------------------\n";
	for (auto obj : objects) {
		std::cout << "obj " << obj->object_name << "layer: "  << obj->layer << " sublayer: " << obj->subLayer << std::endl;
	}*/
}

DrawableObject* Room::FindObject(std::string name) {
	for (auto obj : objects)
		if (obj->object_name == name)
			return obj;
	return nullptr;
}

//puzzle
void Puzzle::Update() {

}

void Puzzle::RightClick(glm::vec3 screen, glm::vec3 world) {

}

void Puzzle::LeftClick(glm::vec3 screen, glm::vec3 world) {

}



Book::Book(int id, std::string texture, int sizeX, int sizeY, int posX, int posY)
{
	this->id = id;
	SetTexture(texture);
	SetSize(sizeX, sizeY);
	SetPosition(glm::vec3(posX, posY, 1));
	prevPos = this->pos;
	this->col = new Collider(this);
}

Book::~Book()
{
	if (col != nullptr)
		delete col;
}

int Book::GetId()
{
	return this->id;
}

void Book::UpdatePrevPos()
{
	//this->prevPos = this->getPos();
}

glm::vec3 Book::GetPrevPos()
{
	return prevPos;
}

bool Book::CheckCollider(int x, int y)
{
	if (this->col->isClicked(x, y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Space::CheckCollide(Collider* col)
{
	if (this->collider->isCollide(col))
	{
		return true;
	}
	else
	{
		return false;
	}
}

BookshelfPuzzle::BookshelfPuzzle(std::string texture, int posX, int posY, int sizeX, int sizeY)
{
	this->texture = new ImageObject();
	this->texture->SetTexture(texture);
	this->texture->SetPosition(glm::vec3(posX, posY, 1));
	this->texture->SetSize(sizeX, sizeY);
	this->win = new ImageObject();
	this->win->SetTexture("Texture/Test/Win.png");
	this->win->SetPosition(glm::vec3(0, 0, 1));
	this->win->SetSize(1080, -720);
}

void BookshelfPuzzle::Init(std::vector<Book*> books, std::vector<Space*> spaces, std::vector<ImageObject*> image)
{
	this->books = books;
	this->log = spaces;
	this->images = image;
}

void BookshelfPuzzle::Render()
{
	if (display)
	{
		for (int i = 0; i < images.size(); i++)
		{
			Game::GetInstance()->GetRenderer()->Render(images[i]);
		}
		for (int j = 0; j < log.size(); j++)
		{
			Game::GetInstance()->GetRenderer()->Render(log[j]->colTemp);
		}
		for (int i = 0; i < books.size(); i++)
		{
			Game::GetInstance()->GetRenderer()->Render(books[i]);
		}
		if (pass == true) {
			Game::GetInstance()->GetRenderer()->Render(win);
		}
	}
}

void BookshelfPuzzle::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	if (select == nullptr)
	{
		for (int i = 0; i < books.size(); i++)
		{
			if (books[i]->col->isClicked(screen.x, screen.y))
			{
				select = books[i];
			}
		}
		for (int j = 0; j < log.size(); j++)
		{
			if (log[j]->collider->isClicked(screen.x, screen.y))
			{
				select = log[j]->book;
				log[j]->book = nullptr;
			}
		}
	}
	else
	{
		bool placed = false;
		for (int i = 0; i < log.size(); i++)
		{
			if (log[i]->collider->isClicked(screen.x, screen.y))
			{
				Book* tmp = log[i]->book;
				log[i]->book = select;
				select->SetPosition(log[i]->collider->getPosition());
				select->col->setNewPos(log[i]->collider->getPosition());
				select->UpdatePrevPos();
				select = tmp;
				placed = true;
				break;
			}
		}
		if (!placed)
		{
			select->SetPosition(select->GetPrevPos());
			select->col->setNewPos(select->GetPrevPos());
			select = nullptr;
		}
	}
	if (pass == true)
	{
		display = false;
	}
}

void BookshelfPuzzle::Update(glm::vec3 screen, glm::vec3 world)
{
	if (select != nullptr)
	{
		select->SetPosition(screen);
		select->col->setNewPos(screen);
	}
	pass = true;
	for (int i = 0; i < log.size(); i++)
	{
		if (log[i]->book == nullptr){
			pass = false;
			break;
		}
		else if (log[i]->id != log[i]->book->GetId())
		{
			pass = false;
			break;
		}
	}
}

BookshelfPuzzle::~BookshelfPuzzle()
{
	for (int i = 0; i < images.size(); i++)
	{
		delete images[i];
	}
	for (int j = 0; j < log.size(); j++)
	{
		delete log[j];
	}
	for (int i = 0; i < books.size(); i++)
	{
		delete books[i];
	}
}
//level

Level::Level(std::string filename) {
	XMLManager* lg = XMLManager::GetInstance();
	lg->GenerateRoom(filename, rooms);
	levelNo = lg->GetLevelNumber(filename);

	// assign first room as current room
	currentRoom = rooms.begin()->second;

	GameScreen* game = ((GameScreen*)Game::GetInstance());
}

void Level::Update() {
	currentRoom->Update();
}

void Level::Render() {
	currentRoom->Render();
}

void Level::RightClick(float x, float y) {
	currentRoom->RightClick(x, y);
}

void Level::LeftClick(float x, float y) {
	currentRoom->LeftClick(x, y);
}

void Level::ChangeRoom(std::string roomName, std::string door) {
	currentRoom = rooms[roomName];

	Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
	if (door != " ") {
		glm::vec3 nextPlayerPosition = glm::vec3(currentRoom->doors[door]->getPos().x, player->getPos().y, 1);
		player->SetPosition(nextPlayerPosition);
	}
	player->SetWalkLimit(currentRoom->GetPlayerWalkLimit());
	player->StopWalking();

	Game::GetInstance()->GetCamera()->SetLimit(currentRoom->GetCameraLimit());
}

void Level::OpenPuzzle(std::string puzzleName) {
	//change game state to puzzle
}

std::vector<DrawableObject*>* Level::Getobjects() {
	return &currentRoom->objects;
}

Level::~Level() {
	for (std::map<std::string, Room*>::iterator itr = rooms.begin(); itr != rooms.end(); itr++) {
		delete itr->second;
	}
}

DrawableObject* Level::FindObject(std::string name) {
	std::map<std::string, Room*>::iterator itr;
	for (itr = rooms.begin(); itr != rooms.end(); itr++) {
		if (itr->second->FindObject(name) != nullptr)
			return itr->second->FindObject(name);
	}
	return nullptr;
}