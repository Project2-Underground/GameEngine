#include "Level.h"
#include "LevelGenerator.h"
#include "Game.h"

void Room::Render() {
	// render objects
	Game::GetInstance()->GetRenderer()->Render(objects);
}

void Room::Update() {

}

void Room::SetRoomLimit(Collider* col) {
	delete roomLimit;
	roomLimit = col;
}

void Room::RightClick(int x, int y) {
	Game* game = Game::GetInstance();
	float realX, realY;
	// temporary while waiting for handle mouse
	int winWidth = 1280;
	int winHeight = 720;
	realX = -(winWidth * 0.5) + x - game->GetCamera()->GetPosition().x;
	realY = -(winHeight * 0.5) + (winHeight - y) - game->GetCamera()->GetPosition().y;
	((GameScreen*)game->GetScreen())->GetPlayer()->SetNextPosition(realX, realY);
}

void Room::LeftClick(int x, int y) {
	Game* game = Game::GetInstance();
	float realX, realY;
	// temporary while waiting for handle mouse
	int winWidth = 1280;
	int winHeight = 720;
	realX = -(winWidth * 0.5) + x - game->GetCamera()->GetPosition().x;
	realY = -(winHeight * 0.5) + (winHeight - y) - game->GetCamera()->GetPosition().y;
	for (int i = 0; i < objects.size(); i++)
	{
		if (InteractableObj * ib = dynamic_cast<InteractableObj*>(objects[i]))
		{
			if (ib->CheckCollider(realX, realY))
				((GameScreen*)game->GetScreen())->GetPlayer()->SetTarget(ib);
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
	LevelGenerator lg;
	lg.GenerateRoom(filename, rooms);

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
	currentRoom->RightClick(x,y);
}

void Level::LeftClick(int x, int y) {
	currentRoom->LeftClick(x,y);
}

void Level::ChangeRoom(std::string roomName) {
	currentRoom = rooms[roomName];
}

void Level::OpenPuzzle(std::string puzzleName) {
	//change game state to puzzle
}

std::vector<DrawableObject*>* Level::Getobjects() {
	return &currentRoom->objects;
}