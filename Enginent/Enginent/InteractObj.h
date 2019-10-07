#pragma once

#include "ImageObject.h"

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
public:
	virtual void action() = 0;
	void setType(IneractTypeList newInteractType) { interactType = newInteractType; };
	IneractTypeList getType() { return interactType; };
};