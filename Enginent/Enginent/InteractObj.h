#pragma once

#include "ImageObject.h"
#include "Collider.h"

enum IneractTypeList
{
	NORMAL = 0,
	VIEW,
	PICKUP,
	TALK,
	MOVESCENE
};

class InteractableObj : public ImageObject {
	IneractTypeList interactType = NORMAL;
	std::string dialogue;
	Collider* col;
public:
	InteractableObj() {};
	InteractableObj(std::string s) { dialogue = s; };
	void SetCollder(Collider* n_col) {
		printf("Col create\n");
		if (dialogue != "")
		{
			printf("%s\n", dialogue);
		}
		col = n_col;
		col->setRefObject(this);
	};
	virtual void action() {};
	void setType(IneractTypeList newInteractType) { interactType = newInteractType; };
	IneractTypeList getType() { return interactType; };
};

