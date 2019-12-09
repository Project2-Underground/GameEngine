#include "Level.h"
#include "LevelGenerator.h"

void Room::Render() {
	// render objects
}

void Room::OnMouseDown() {

}

void Room::OnMouseUp() {

}

void Room::OnMouseDrag() {

}

//puzzle
void Puzzle::OnMouseDown() {

}

void Puzzle::OnMouseUp() {

}

void Puzzle::OnMouseDrag() {

}

//level

Level::Level(std::string filename) {
	LevelGenerator lg;
	lg.GenerateRoom(filename, rooms);

	// assign first room as current room
	currentRoom = rooms.begin()->second;
}

void Level::Render() {
	currentRoom->Render();
}

void Level::ChangeRoom(std::string roomName) {
	currentRoom = rooms[roomName];
}

void Level::OpenPuzzle(std::string puzzleName) {
	//change game state to puzzle
}