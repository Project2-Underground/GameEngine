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
	PLAYER_TRIGGER,
	ADDNOTE
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

	bool takeNote;
	bool hasNextTexture;
	bool talk;
	std::string noteName;
	std::string item_to_use;
	std::string dialogue_name;
	std::string dialogue_before;
	std::string dialogue_after;
	std::string itemName;
	unsigned int nextTexture; // after picking up an item

public:
	bool used;
	bool triggered;
	bool hasItem;
	InteractableObj();

	virtual void action();
	void SetAnimation(std::string name, std::string texture, int frameNo, float frameRate, bool loop = false);
	void SetCollder(Collider* n_col);
	void SetType(InteractTypeList newInteractType) { interactType = newInteractType; };
	void SetNoteName(std::string);
	void SetDialogueName(std::string, std::string);
	bool CheckCollider(float x, float y);
	bool CheckPointing(float x, float y);
	void SetInteractType(InteractTypeList type) { interactType = type; }
	void SetItemToUse(std::string item_to_unlock);
	void SetNextTexture(std::string);
	void SetItem(std::string item) { itemName = item; hasItem = true; }
	Item* GetItem();
	void TakeNote();
	void PickUpItem();
	virtual void UseItem(Item* item);
	void ChangeDialogue(std::string n, std::string a);
	std::string GetCurrentDialogueName() { return dialogue_name; }
	std::string GetDialogueBeforeName() { return dialogue_before; }
	std::string GetDialogueAfterName() { return dialogue_after; }
	void SetCurrentDialogueName(std::string d) { dialogue_name = d; }
	void SetDialogueBeforeName(std::string d) { dialogue_before = d; }
	void SetDialogueAfterName(std::string d) { dialogue_after = d; }
	void SetTalked(bool b);
	void SetTakeNote(bool b) { takeNote = b; }
	bool Talked() { return talk; }
	bool TookNote() { return takeNote; }

	InteractTypeList getType() { return interactType; };

	~InteractableObj();
	bool operator==(const InteractableObj& obj);
};

class OpenObj : public InteractableObj {
	std::string sound;
	bool open;
	unsigned int openTexture;
public:
	OpenObj();

	void SetOpenTexture(std::string);
	void action();
	bool IsOpen() { return open; }
	void SetSound(std::string);

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
	bool talk;
public:
	NonPlayer(std::string name) { object_name = name; interactType = TALK; talk = false; }
	void action();
};

class SaveObj : public InteractableObj {
public:
	SaveObj() { interactType = SAVE; }
	void action();
};

class PlayerTriggerObj : public InteractableObj {
protected:
	bool clickToInteract;
	bool disappearAfterAction;
public:
	PlayerTriggerObj();
	void Update();
};

class NumpadPuzzleAfter : public InteractableObj {
	bool actionDone;
public:
	NumpadPuzzleAfter();
	void action();
	void UnlockBookshelf();
};