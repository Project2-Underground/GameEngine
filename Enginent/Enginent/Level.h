#pragma once

#include "DrawableObject.h"
#include <map>
#include <vector>

class Room {
public:
	std::vector<DrawableObject*> objects;

	void Render();
	virtual void Update();
	virtual void OnMouseDown();
	virtual void OnMouseUp();
	virtual void OnMouseDrag();
};

class Puzzle :public Room {
public:
	void Update();
	void OnMouseDown();
	void OnMouseUp();
	void OnMouseDrag();
};


class Level {
	std::map<std::string, Room*> rooms;
	std::map<std::string, Puzzle*> puzzles;
	Room* currentRoom;
public:
	Level(std::string);
	void Render(); 
	void Update();
	void ChangeRoom(std::string roomName);
	void OpenPuzzle(std::string puzzleName);
};