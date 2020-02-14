#pragma once

#include "ImageObject.h"
#include "Collider.h"
#include "Item.h"
#include "TextBox.h"

enum InteractTypeList
{
	NORMAL = 1,
	VIEW,
	PICKUP,
	DOOR,
	TALK,
	CHANGESCENE,
	SAVE,
	OPEN,
	PUZZLE
};

class InteractableObj : public ImageObject {
protected:
	InteractTypeList interactType = NORMAL;
	vector<Dialogue> dialogue;
	int currDialogue = 0;
	Collider* col;
	std::string interact_sound = "";

	bool takePic;
	std::string picName;
	std::string item_to_use;
	Item* item;
public:
	InteractableObj() {};
	InteractableObj(vector<Dialogue> s);

	virtual void action();
	void SetSound(std::string);
	void SetDialogue(vector<Dialogue> s);
	void SetCollder(Collider* n_col);
	void SetType(InteractTypeList newInteractType) { interactType = newInteractType; };
	void SetTakePic(std::string);
	bool CheckCollider(float x, float y);
	bool CheckPointing(float x, float y);
	int GetCurrentDialogue() { return currDialogue; }
	void SetCurrentDialogue(int num) { currDialogue = num; }
	void SetItemToUse(std::string item_to_unlock);
	void SetItem(Item* item) { this->item = item; }
	Item* GetItem() { return item; }
	void TakePic();
	void PickUpItem();
	void UseItem(Item* item);

	InteractTypeList getType() { return interactType; };

	~InteractableObj();
	bool operator==(const InteractableObj& obj);
	bool used;
};

class OpenObj : public InteractableObj {
	bool open;
	unsigned int openTexture;
	unsigned int nextTexture; // after picking up an item
public:
	OpenObj();

	void SetOpenTexture(std::string);
	void SetNextTexture(std::string);
	void action();

	~OpenObj();
};

class ViewObj : public InteractableObj {
	unsigned int viewTexture;
public:
	std::string description;

	ViewObj() { interactType = VIEW; }

	void SetViewTexture(std::string);
	void action();
};

class PuzzleObj : public InteractableObj {
	std::string puzzleName;
public:
	PuzzleObj() { interactType = PUZZLE; }
	void SetPuzzleName(std::string);
	void action();
};

class NonPlayer : public InteractableObj {
public:
	NonPlayer();
	void action();
};

class Door : public InteractableObj {
	std::string nextRoom;
	std::string nextDoor;
public:
	Door(std::string, std::string);
	void action();
};