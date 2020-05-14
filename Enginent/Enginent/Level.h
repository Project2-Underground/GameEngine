#pragma once

#include "Door.h"
#include "UIObject.h"
#include "Collider.h"
#include "Item.h"
#include "Puzzles.h"

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
	std::vector<DrawableObject*> npcs;
	std::map<std::string, Door*> doors;
	std::string name;
	std::string dialogue;
	std::string bgmName;

	void Render();
	void Update();
	void RightClick(float, float);
	void LeftClick(float, float);
	void SetPlayerWalkLimit(Collider*);
	void SetCameraLimit(Collider*);
	DrawableObject* FindObject(std::string);
	Collider* GetPlayerWalkLimit() {return playerWalkLimit;}
	Collider* GetCameraLimit() { return cameraLimit; };

	float y;
	glm::vec3 playerNextPosition;
	void SortObjLayer();

	~Room();
};

class Level {
	Room* currentRoom;
public:
	int levelNo;
	float xStart;
	std::map<std::string, Room*> rooms;
	Level(std::string);
	void Render();
	void Update();
	void RightClick(float, float);
	void LeftClick(float, float);

	void ChangeRoom(std::string roomName, std::string door = "");
	void TriggerChangeDialogue(std::string objName, std::string dName);

	std::vector<DrawableObject*>* Getobjects();
	Room* GetCurrentRoom() { return currentRoom; };
	DrawableObject* FindObject(std::string);

	~Level();
};