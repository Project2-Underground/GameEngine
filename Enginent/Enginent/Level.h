#pragma once

#include "InteractObj.h"
#include "Collider.h"
#include "Item.h"
#include <map>
#include <vector>

enum Layers {
	BACKGROUND_LAYER = 0,
	OBJECT_LAYER,
	NPC_LAYER,
	PLAYER_LAYER,
	FOREGROUND_LAYER
};

class Room {
	Collider* playerWalkLimit;
	Collider* cameraLimit;
public:
	std::vector<Item*> items;
	std::vector<DrawableObject*> objects;
	std::vector<DrawableObject*> foreground;
	std::map<std::string, Door*> doors;
	std::string name;

	void Render();
	virtual void Update();
	virtual void RightClick(int, int);
	virtual void LeftClick(int, int);
	void SetPlayerWalkLimit(Collider*);
	void SetCameraLimit(Collider*);
	DrawableObject* FindObject(std::string);
	Collider* GetPlayerWalkLimit() { return playerWalkLimit; };
	Collider* GetCameraLimit() { return cameraLimit; };

	void SortObjLayer();

	~Room();
};

class Puzzle :public Room {
public:
	void Update();
	void RightClick();
	void LeftClick();
};


class Level {
	Room* currentRoom;
public:
	int levelNo;
	std::map<std::string, Room*> rooms;
	std::map<std::string, Puzzle*> puzzles;
	Level(std::string);
	void Render();
	void Update();
	void RightClick(int, int);
	void LeftClick(int, int);

	void ChangeRoom(std::string roomName, std::string door = " ");
	void OpenPuzzle(std::string puzzleName);

	std::vector<DrawableObject*>* Getobjects();
	Room* GetCurrentRoom() { return currentRoom; };
	DrawableObject* FindObject(std::string);

	~Level();
};