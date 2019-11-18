#pragma once

#include "ImageObject.h"
#include "Collider.h"

enum IneractTypeList
{
	NORMAL = 0,
	VIEW,
	PICKUP,
	TALK,
	DOOR
};

class InteractableObj : public ImageObject {
protected:
	IneractTypeList interactType = NORMAL;
	vector<std::string>* dialogue;
	int currDialogue = 0;
	Collider* col;
public:
	InteractableObj() {};
	InteractableObj(vector<std::string>* s);

	virtual void action(int x, int y);

	void SetDialogue(vector<std::string>* s);
	void SetCollder(Collider* n_col);
	void setType(IneractTypeList newInteractType) { interactType = newInteractType; };
	void checkCollider(int x, int y);

	IneractTypeList getType() { return interactType; };

	~InteractableObj();
};

