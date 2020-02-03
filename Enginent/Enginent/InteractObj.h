#pragma once

#include "ImageObject.h"
#include "Collider.h"
#include "Item.h"

enum InteractTypeList
{
	NORMAL = 1,
	VIEW,
	PICKUP,
	DOOR,
	TALK,
	CHANGESCENE,
	SAVE,
	OPEN
};

class InteractableObj : public ImageObject {
protected:
	InteractTypeList interactType = NORMAL;
	vector<std::string> dialogue;
	int currDialogue = 0;
	Collider* col;
	std::string interact_sound = "";

	bool takePic;
	std::string picName;
public:
	InteractableObj() {};
	InteractableObj(vector<std::string> s);

	virtual void action();
	void SetSound(std::string);
	void SetDialogue(vector<std::string> s);
	void SetCollder(Collider* n_col);
	void SetType(InteractTypeList newInteractType) { interactType = newInteractType; };
	void SetTakePic(std::string);
	bool CheckCollider(float x, float y);
	bool CheckPointing(float x, float y);
	int GetCurrentDialogue() { return currDialogue; }
	void SetCurrentDialogue(int num) { currDialogue = num; }

	InteractTypeList getType() { return interactType; };

	~InteractableObj();
	bool operator==(const InteractableObj& obj);
};

class OpenObj : public InteractableObj {
	bool open;
	unsigned int openTexture;
	unsigned int nextTexture; // after picking up an item
	Item* item;
public:
	OpenObj();

	void SetOpenTexture(std::string);
	void SetNextTexture(std::string);
	void SetItem(Item* item) { this->item = item; }
	Item* GetItem() { return item; }
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

class UseItemObj :public InteractableObj {
protected:
	std::string item_to_use;
public:
	bool used;
	void SetItemToUse(std::string item_to_unlock);
};

class NonPlayer : public InteractableObj {
public:
	NonPlayer();
	void action();
};

class Door : public UseItemObj {
	std::string nextRoom;
	std::string nextDoor;
public:
	Door(std::string, std::string);
	//Door() {};
	void action();
	void UseItem(Item* item);
};