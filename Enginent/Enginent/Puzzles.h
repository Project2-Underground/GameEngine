#pragma once

#include <vector>
#include "InteractObj.h"
#include "UIObject.h"
#include "Button.h"


class PuzzleTemplate : public UIObject {
protected:
	bool pass;
	bool doneAction;
	std::string dialogueAfterComplete;
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
	int ID;
	bool select;
	bool paste;
	unsigned int book_paperTexture;
	unsigned int norm_paperTexture;
	ImageObject* book_paper;
	glm::vec3 prevPos;
	Paper(std::string n_texture, std::string b_texture, glm::vec3 Pos, float SizeX, float SizeY, int id)
	{
		this->ID = id;
		select = false;
		paste = false;
		book_paper = new ImageObject();
		book_paper->SetTexture(b_texture);
		book_paperTexture = book_paper->GetTexture();
		book_paper->SetTexture(n_texture);
		norm_paperTexture = book_paper->GetTexture();
		SetTexture(norm_paperTexture);
		SetPosition(Pos);
		SetSize(SizeX, SizeY);
		prevPos = Pos;
		this->col = new Collider(this);
	}
	bool CheckCollider(float x, float y)
	{
		if (this->col->isClicked(x, y))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

class Book2 : public UIObject {
	int paper_id;
	int id;
	glm::vec3 prevPos;
public:
	Paper* paper;
	Book2(int id, std::string texture, float sizeX, float sizeY, int posX, int posY, int paperID);
	~Book2();
	bool checkPaper();
	int GetId()
	{
		return id;
	}
	int GetPaperid()
	{
		return paper_id;
	}
	glm::vec3 GetPrevPos()
	{
		return prevPos;
	}
	bool CheckCollider(float x, float y)
	{
		if (this->col->isClicked(x, y))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
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

struct Space2 {
	UIObject* colTemp;
	Book2* book = nullptr;
	Collider* collider;
	int sizeX = 10;
	int sizeY = 10;
	int posX = 0;
	int posY = 0;
	int id;
	bool CheckCollide(Collider* col)
	{
		if (this->collider->isCollide(col))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	Space2(int id, glm::vec3 pos, float sizeX, float sizeY)
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
	~Space2()
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

class Bookshelf2 : public PuzzleTemplate {
	UIObject* texture;
	std::vector<UIObject*> images;
	std::vector<Space*> log;
	std::vector<Book2*> books_2;
	std::vector<Book*> books;
	std::vector<Paper*> papers;
	Book* select = nullptr;
	Book* hold = nullptr;
	Space* select_s = nullptr;
	Paper* select_p = nullptr;
	Book2* select_r = nullptr;

	Book2* FindBook2(int id);
	Paper* FindPaper(int id);
	Book* FindBook(int id);
public:
	Bookshelf2(std::string, int posX, int posY, int sizeX, int sizeY);
	void Init(std::vector<Book*>, std::vector<Book2*>, std::vector<Space*>, std::vector<UIObject*>, std::vector<Paper*>);
	void Render();
	void Update();
	void Reset();
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void RightClick(glm::vec3, glm::vec3);
	void RightRelease(glm::vec3, glm::vec3);
	void ActionAfterPuzzle();
	void CompletePuzzle();
	~Bookshelf2();
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
	void Reset();
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
	Numpad2(std::string texture, int posX, int posY, int sizeX, int sizeY) :Numpad(texture, posX, posY, sizeX, sizeY) { dialogueAfterComplete.clear(); }
	void CompletePuzzle();
	void ActionAfterPuzzle();
};

//================================================================================================

class Puzzle {
protected:
	int currentLevel;
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
	int GetPuzzleLevel() { return currentLevel; }
	~Puzzle();

	std::string prepTalk;
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

class BookshelfPuzzle_2 : public Puzzle {
public:
	BookshelfPuzzle_2();
	void Render();
	void Update();
	void Reset();
	bool CheckRequirements();
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void RightClick(glm::vec3, glm::vec3);
	void RightRelease(glm::vec3, glm::vec3);
	void UpdateMouseState(glm::vec3, glm::vec3);
	void CompletePuzzle();
	~BookshelfPuzzle_2();
};

class NumpadPuzzle : public Puzzle {
public:
	NumpadPuzzle();
	void Render();
	void Update();
	void Reset();
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
	void Reset();
	bool CheckRequirements();
	void LeftClick(glm::vec3, glm::vec3);
	void LeftRelease(glm::vec3, glm::vec3);
	void RightClick(glm::vec3, glm::vec3) {}
	void UpdateMouseState(glm::vec3, glm::vec3);
	void CompletePuzzle();
	~NumpadPuzzle_2();
};