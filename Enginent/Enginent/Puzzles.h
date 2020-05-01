#pragma once

#include <vector>
#include "InteractObj.h"
#include "UIObject.h"
#include "Button.h"


class PuzzleTemplate : public UIObject {
protected:
	bool pass;
	bool doneAction;
public:
	virtual void Render() {};
	virtual void Update() {};
	virtual void Reset() {};
	virtual void LeftClick(glm::vec3, glm::vec3) {};
	virtual void LeftRelease(glm::vec3, glm::vec3) {};
	virtual void FinishedPuzzle() {};
	virtual void ActionAfterPuzzle() {};
	virtual void CompletePuzzle() {};
	bool Passed() { return pass; }
};

struct Paper : public UIObject {
	Paper();
	~Paper();
	int ID;
	glm::vec3 prevPos;
	bool CheckCollider(float x, float y);
};

class Book2 : public UIObject {
	int id;
	glm::vec3 prevPos;
	Paper* paper;
	int paper_id;
public:
	Book2(int id, std::string texture, float sizeX, float sizeY, int posX, int posY, int paperID);
	~Book2();
	void UpdatePrevPos();
	int GetId();
	glm::vec3 GetPrevPos();
	bool CheckCollider(float x, float y);
	bool checkPaper();
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
	std::vector<UIObject*> images;
	std::vector<Space*> log;
	std::vector<Book*> books;
	Book* select = nullptr;
public:
	Bookshelf(std::string, int posX, int posY, int sizeX, int sizeY);
	void Init(std::vector<Book*>, std::vector<Space*>, std::vector<UIObject*>);
	void Render();
	void Update(); 
	void Reset();
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void ActionAfterPuzzle();
	void CompletePuzzle();
	~Bookshelf();
};

//////////////////////////////////////////////
// Numpad Puzzle
class Numpad;

class NumberButton : public Button {
	std::vector<int>* input;
	int input_size;
	int num;
public:
	NumberButton(std::string texture, std::string press, int n, float posX, float posY, float sizeX, float sizeY, std::vector<int>* input, int input_size);
	int GetNum() { return num; }
	void action();
};

class EnterButton : public Button {
	Numpad* numpad;
public:
	EnterButton(std::string texture, std::string press, float posX, float posY, float sizeX, float sizeY, Numpad* n);
	void action();
};

class DeleteButton : public Button {
	Numpad* numpad;
public:
	DeleteButton(std::string texture, std::string press, float posX, float posY, float sizeX, float sizeY, Numpad* n);
	void action();
};

class Numpad : public PuzzleTemplate{
protected:
	UIObject* texture;
	std::vector<UIObject*> image;
	std::vector<Button*> n_button; //0-9 and 10,11 for enter/delete 
	int* cheat;
	std::vector<int>* input;
	int cheat_size;
	SDL_Color numColor = { 0, 0, 0, 0 };
	TextObject* numUI;
public:
	Numpad(std::string, int posX, int posY, int sizeX, int sizeY);
	void Init(std::vector<UIObject*>, std::vector<Button*>, int* c, int c_size, std::vector<int>* input);
	void Render();
	void Update();
	void Reset() {};
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void RightClick(glm::vec3, glm::vec3) {}
	void ActionAfterPuzzle();
	void CompletePuzzle();
	void CheckCheat();
	void deleteInput();
	void setNumColor(SDL_Color c)
	{
		this->numColor = c;
	}
	~Numpad();
};

class Numpad2 : public Numpad {
public:
	Numpad2(std::string texture, int posX, int posY, int sizeX, int sizeY) :Numpad(texture, posX, posY, sizeX, sizeY) {}
	void CompletePuzzle();
	void ActionAfterPuzzle();
};

class Puzzle {
protected:
	std::vector<UIObject*> UI;
	UIObject* background;
	PuzzleTemplate* puzzle;
	bool enableInventory;
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Reset() = 0;
	virtual bool CheckRequirements() = 0;
	virtual void RightClick(glm::vec3, glm::vec3) = 0;
	virtual void LeftClick(glm::vec3, glm::vec3) = 0;
	virtual void LeftRelease(glm::vec3, glm::vec3) = 0;
	virtual void UpdateMouseState(glm::vec3, glm::vec3) = 0;
	virtual void CompletePuzzle() = 0;
	bool IsInventoryEnable() { return enableInventory; }
	bool Passed() { return puzzle->Passed(); }
	bool passedReqiurements;
	~Puzzle();
};

class BookshelfPuzzle : public Puzzle {
public:
	BookshelfPuzzle();
	void Render();
	void Update();
	void Reset();
	bool CheckRequirements();
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void RightClick(glm::vec3, glm::vec3) {}
	void UpdateMouseState(glm::vec3, glm::vec3);
	void CompletePuzzle();
	~BookshelfPuzzle();
};

class NumpadPuzzle : public Puzzle {
public:
	NumpadPuzzle();
	void Render();
	void Update();
	void Reset() {};
	bool CheckRequirements();
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void RightClick(glm::vec3, glm::vec3) {}
	void UpdateMouseState(glm::vec3, glm::vec3);
	void CompletePuzzle();
	~NumpadPuzzle();
};

class NumpadPuzzle_2 : public Puzzle {
public:
	NumpadPuzzle_2();
	void Render();
	void Update();
	void Reset() {};
	bool CheckRequirements();
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void RightClick(glm::vec3, glm::vec3) {}
	void UpdateMouseState(glm::vec3, glm::vec3);
	void CompletePuzzle();
	~NumpadPuzzle_2();
};