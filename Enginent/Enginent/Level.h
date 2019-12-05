#pragma once

#include "DrawableObject.h"
#include <map>
#include <vector>

class Room {
	std::vector<DrawableObject*> objects;
public:
	Room(std::string);
	void OnRender();
	virtual void OnMouseDown();
	virtual void OnMouseUp();
	virtual void OnMouseDrag();
};

class Puzzle :public Room {
	std::vector<DrawableObject*> objects;
public:
	Puzzle(std::string puzzleName);
	void OnRender();
	virtual void OnMouseDown();
	virtual void OnMouseUp();
	virtual void OnMouseDrag();
};


class Level {
	std::map<std::string, Room*> rooms;
	std::map<std::string, Puzzle*> puzzles;
	Room* currentRoom;
public:
	Level(std::string[]);
	void Init();
	void OnRender();
	void ChangeRoom(std::string roomName);
	void OpenPuzzle(std::string puzzleName);
};