#pragma once

#include "DrawableObject.h"
#include "Collider.h"
#include <map>
#include <vector>

class Room {
	Collider* roomLimit;
public:
	std::vector<DrawableObject*> objects;

	void Render();
	virtual void Update();
	virtual void RightClick(int, int);
	virtual void LeftClick(int, int);
	void SetRoomLimit(Collider*);
	Collider* GetRoomLimit() { return roomLimit; };
};

class Puzzle :public Room {
public:
	void Update();
	void RightClick();
	void LeftClick();
};


class Level {
	std::map<std::string, Room*> rooms;
	std::map<std::string, Puzzle*> puzzles;
	Room* currentRoom;
public:
	Level(std::string);
	void Render();
	void Update();
	void RightClick(int, int);
	void LeftClick(int, int);

	void ChangeRoom(std::string roomName);
	void OpenPuzzle(std::string puzzleName);
	std::vector<DrawableObject*>* Getobjects();
	Room* GetCurrentRoom() { return currentRoom; };
};