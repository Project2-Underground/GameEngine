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
	PUZZLE,
	STAIR,
	PLAYER_TRIGGER
};

enum DialogueTrigger 
{
	ACTION_TRIGGER_DIALOGUE = 0,
	USE_ITEM_TRIGGER_DIALOGUE
};

class InteractableObj : public ImageObject {
protected:
	InteractTypeList interactType;
	std::string interact_sound = "";

	bool takePic;
	bool hasNextTexture;
	std::string picName;
	std::string item_to_use;
	std::string dialogue_name;
	std::string itemName;
	std::vector<InteractableObj*> triggerObjs;
	unsigned int nextTexture; // after picking up an item

public:
	InteractableObj();

	virtual void action();
	void SetAnimation(std::string name, std::string texture, int frameNo, float frameRate, bool loop = false);
	void SetCollder(Collider* n_col);
	void SetType(InteractTypeList newInteractType) { interactType = newInteractType; };
	void SetTakePic(std::string);
	void SetDialogueName(std::string);
	bool CheckCollider(float x, float y);
	bool CheckPointing(float x, float y);
	void SetInteractType(InteractTypeList type) { interactType = type; }
	void SetItemToUse(std::string item_to_unlock);
	void SetNextTexture(std::string);
	void SetItem(std::string item) { itemName = item; hasItem = true; }
	Item* GetItem();
	void TakePic();
	void PickUpItem();
	virtual void UseItem(Item* item);
	void AddTriggerObj(InteractableObj*);
	void ChangeDialogue(std::string n);
	std::string GetCurrentDialogueName() { return dialogue_name; }

	InteractTypeList getType() { return interactType; };

	~InteractableObj();
	bool operator==(const InteractableObj& obj);
	bool used;
	bool triggered;
	bool hasItem;
};

class OpenObj : public InteractableObj {
	bool open;
	unsigned int openTexture;
public:
	OpenObj();

	void SetOpenTexture(std::string);
	void action();
	bool IsOpen() { return open; }

	void Open();
	void ClearItem();

	~OpenObj();
};

class ViewObj : public InteractableObj {
	unsigned int viewTexture;
public:
	std::string description;

	ViewObj() { interactType = VIEW; }

	float width;
	float height;

	void SetViewTexture(std::string);
	unsigned int GetViewTexture();
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
	bool giveItem;
public:
	NonPlayer(std::string name) { object_name = name; interactType = TALK; }
	void action();
};

class SaveObj : public InteractableObj {
public:
	SaveObj() { interactType = SAVE; }
	void action();
};

class PlayerTriggerObj : public InteractableObj {
public:
	void Update();
};