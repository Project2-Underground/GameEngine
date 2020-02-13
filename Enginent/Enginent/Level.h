#pragma once

#include "InteractObj.h"
#include "UIObject.h"
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
	virtual void RightClick(float, float);
	virtual void LeftClick(float, float);
	void SetPlayerWalkLimit(Collider*);
	void SetCameraLimit(Collider*);
	DrawableObject* FindObject(std::string);
	Collider* GetPlayerWalkLimit() { return playerWalkLimit; };
	Collider* GetCameraLimit() { return cameraLimit; };

	void SortObjLayer();

	~Room();
};

class Puzzle : public UIObject{
public:
	virtual void Update();
	virtual void RightClick(glm::vec3, glm::vec3);
	virtual void LeftClick(glm::vec3, glm::vec3);
};

class Book : public InteractableObj{
	int id;
	glm::vec3 prevPos;
public:
	Collider* col = nullptr;
	Book(int id, std::string texture, int sizeX, int sizeY, int posX, int posY);
	~Book();
	void UpdatePrevPos();
	int GetId();
	glm::vec3 GetPrevPos();
	bool CheckCollider(int x, int y);
};

struct Space {
	UIObject* colTemp;
	Book* book = nullptr;
	Collider* collider;
	int sizeX = 10;
	int sizeY = 10;
	int posX = 0;
	int posY = 0;
	int id;
	bool CheckCollide(Collider*);
	Space(int id, glm::vec3 pos, int sizeX, int sizeY)
	{
		this->id = id;
		collider = new Collider();
		collider->setNewPos(pos);
		collider->setNewSize(sizeX, sizeY);
		colTemp = new UIObject();
		colTemp->SetTexture("Texture/Test/white.png");
		colTemp->SetPosition(collider->getPosition());
		colTemp->SetSize(sizeX, sizeY);
	}
	~Space()
	{
		delete colTemp;
	}
};

class BookshelfPuzzle : public Puzzle {
	ImageObject* texture;
	ImageObject* win;
	std::vector<ImageObject*> images;
	std::vector<Space*> log;
	std::vector<Book*> books;
	Book* select = nullptr;
	bool pass = false;
	bool display = true;
public:
	BookshelfPuzzle(std::string, int posX, int posY, int sizeX, int sizeY);
	void Init(std::vector<Book*>, std::vector<Space*>, std::vector<ImageObject*>);
	void Render();
	void Update(glm::vec3, glm::vec3);
	void LeftClick(glm::vec3, glm::vec3);
	~BookshelfPuzzle();
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
	void RightClick(float, float);
	void LeftClick(float, float);

	void ChangeRoom(std::string roomName, std::string door = " ");
	void OpenPuzzle(std::string puzzleName);

	std::vector<DrawableObject*>* Getobjects();
	Room* GetCurrentRoom() { return currentRoom; };
	DrawableObject* FindObject(std::string);

	~Level();
};