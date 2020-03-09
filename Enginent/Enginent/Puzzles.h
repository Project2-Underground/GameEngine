#pragma once

#include "InteractObj.h"
#include "UIObject.h"

class PuzzleTemplate : public UIObject {
protected:
	bool pass;
	bool doneAction;
public:
	virtual void Render() {};
	virtual void Update() {};
	virtual void LeftClick(glm::vec3, glm::vec3) {};
	virtual void LeftRelease(glm::vec3, glm::vec3) {};
	virtual void FinishedPuzzle() {};
	virtual void ActionAfterPuzzle() {};
	virtual void CompletePuzzle() {};
};

class Book : public UIObject {
	int id;
	glm::vec3 prevPos;
public:
	Book(int id, std::string texture, float sizeX, float sizeY, int posX, int posY);
	~Book();
	void UpdatePrevPos();
	int GetId();
	glm::vec3 GetPrevPos();
	bool CheckCollider(float x, float y);
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
	Space(int id, glm::vec3 pos, float sizeX, float sizeY)
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

class Bookshelf : public PuzzleTemplate {
	UIObject* texture;
	UIObject* win;
	std::vector<UIObject*> images;
	std::vector<Space*> log;
	std::vector<Book*> books;
	Book* select = nullptr;
public:
	Bookshelf(std::string, int posX, int posY, int sizeX, int sizeY);
	void Init(std::vector<Book*>, std::vector<Space*>, std::vector<UIObject*>);
	void Render();
	void Update();
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void ActionAfterPuzzle();
	void CompletePuzzle();
	~Bookshelf();
};

class Puzzle {
protected:
	std::vector<UIObject*> UI;
	UIObject* background;
	bool enableInventory;
	bool pass;
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void RightClick(glm::vec3, glm::vec3) = 0;
	virtual void LeftClick(glm::vec3, glm::vec3) = 0;
	virtual void LeftRelease(glm::vec3, glm::vec3) = 0;
	virtual void UpdateMouseState(glm::vec3, glm::vec3) = 0;
	virtual void CompletePuzzle() = 0;
	bool IsInventoryEnable() { return enableInventory; }
	bool Passed() { return pass; }
};

class BookshelfPuzzle : public Puzzle {
	Bookshelf* puzzle;
public:
	BookshelfPuzzle();
	void Render();
	void Update();
	void RightClick(glm::vec3, glm::vec3) {};
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void UpdateMouseState(glm::vec3, glm::vec3);
	void CompletePuzzle();
	~BookshelfPuzzle();
};