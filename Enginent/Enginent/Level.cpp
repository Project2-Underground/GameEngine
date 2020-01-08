#include "Level.h"
#include "LevelGenerator.h"
#include "Game.h"

void Room::Render() {
	// render objects
	Game::GetInstance()->GetRenderer()->Render(objects);
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
	float realX, realY;
	// temporary while waiting for handle mouse
	int winWidth = 1280;
	int winHeight = 720;
	realX = -(winWidth * 0.5f) + x - game->GetCamera()->GetPosition().x;
	realY = -(winHeight * 0.5f) + (winHeight - y) - game->GetCamera()->GetPosition().y;
	((GameScreen*)game->GetScreen())->GetPlayer()->SetNextPosition(realX, realY);
}

void Room::LeftClick(int x, int y) {
	Game* game = Game::GetInstance();
	float realX, realY;
	// temporary while waiting for handle mouse
	int winWidth = 1280;
	int winHeight = 720;
	realX = -(winWidth * 0.5f) + x - game->GetCamera()->GetPosition().x;
	realY = -(winHeight * 0.5f) + (winHeight - y) - game->GetCamera()->GetPosition().y;
	for (int i = 0; i < objects.size(); i++)
	{
		if (InteractableObj * ib = dynamic_cast<InteractableObj*>(objects[i]))
		{
			if (ib->CheckCollider(realX, realY)) {
				((GameScreen*)game->GetScreen())->GetPlayer()->CheckTarget(ib);
			}
		}
	}
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
	LevelGenerator* lg = LevelGenerator::GetInstance();
	lg->GenerateRoom(filename, rooms);
	levelNo = lg->GetLevelNumber(filename);

	// assign first room as current room
	currentRoom = rooms.begin()->second;
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
	glm::vec3 nextPlayerPosition = glm::vec3(currentRoom->doors[door]->getPos().x, player->getPos().y, 1);
	player->SetPosition(nextPlayerPosition);
	player->SetWalkLimit(currentRoom->GetPlayerWalkLimit());
	player->StopWalking();

	Game::GetInstance()->GetCamera()->SetLimit(currentRoom->GetCameraLimit());
}

void Level::LoadLevel(std::string filename) {
	LevelGenerator::GetInstance()->LoadFromSave(filename, rooms);
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