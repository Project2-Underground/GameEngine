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
	IneractTypeList interactType;
	vector<std::string> dialogue;
	int currDialogue = 0;
	Collider* col;
public:

	InteractableObj() {};
	InteractableObj(vector<std::string> s);

	virtual void action();

	void SetDialogue(vector<std::string> s);
	void SetCollder(Collider* n_col);
	void SetType(IneractTypeList newInteractType) { interactType = newInteractType; };
	bool CheckCollider(int x, int y);
	bool CheckPointing(int x, int y);

	IneractTypeList getType() { return interactType; };

	~InteractableObj();
	bool operator==(const InteractableObj& obj);
};

