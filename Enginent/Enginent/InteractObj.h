#pragma once

#include "ImageObject.h"
#include "Collider.h"

enum IneractTypeList
{
	NORMAL = 1,
	VIEW,
	PICKUP,
	TALK,
	CHANGESCENE,
	SAVE,
	BUTTON
};

class InteractableObj : public ImageObject {
	IneractTypeList interactType = NORMAL;
	vector<std::string>* dialogue;
	int currDialogue = 0;
	Collider* col;
public:
	InteractableObj() {};
	InteractableObj(IneractTypeList type);
	InteractableObj(IneractTypeList type, std::string);
	InteractableObj(IneractTypeList type, vector<std::string>* s);

	virtual void action(int x, int y);

	void SetCollder(Collider* n_col);
	void setType(IneractTypeList newInteractType) { interactType = newInteractType; };
	void checkCollider(int x, int y);
	bool checkPointing(int x, int y);

	IneractTypeList getType() { return interactType; };
};

