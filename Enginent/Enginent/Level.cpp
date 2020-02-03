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

void Room::RightClick(int x, int y) {
	Game* game = Game::GetInstance();
	((GameScreen*)game->GetScreen())->GetPlayer()->SetNextPosition(x, y);
}

void Room::LeftClick(int x, int y) {
	Game* game = Game::GetInstance();

	for (int i = 0; i < objects.size(); i++)
		if (InteractableObj * ib = dynamic_cast<InteractableObj*>(objects[i]))
			if (ib->CheckCollider(x, y)) 
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

void Puzzle::RightClick() {

}

void Puzzle::LeftClick() {

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

void Level::RightClick(int x, int y) {
	currentRoom->RightClick(x, y);
}

void Level::LeftClick(int x, int y) {
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