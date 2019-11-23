#pragma once

#include "ImageObject.h"
#include "Collider.h"

enum IneractTypeList
{
	NORMAL = 1,
	VIEW,
	PICKUP,
	DOOR,
	TALK,
	CHANGESCENE,
	SAVE,
	BUTTON
};

class InteractableObj : public ImageObject {
protected:
	IneractTypeList interactType = NORMAL;
	vector<std::string> dialogue;
	int currDialogue = 0;
	Collider* col;
	std::string interact_sound = "";
public:
	InteractableObj() {};
	InteractableObj(IneractTypeList type);
	InteractableObj(IneractTypeList type, std::string);
	InteractableObj(IneractTypeList type, vector<std::string> s);

	virtual void action();
	void SetSound(std::string);
	void SetDialogue(vector<std::string> s);
	void SetCollder(Collider* n_col);
	void setType(IneractTypeList newInteractType) { interactType = newInteractType; };
	bool checkCollider(int x, int y);
	bool checkPointing(int x, int y);

	IneractTypeList getType() { return interactType; };

	~InteractableObj();
};

