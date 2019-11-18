#pragma once

#include "InteractObj.h"
#include "Collider.h"

class Door : public InteractableObj {
protected:
	IneractTypeList interactType = DOOR;
	glm::vec3 nextPlayerPos;
	glm::vec3 nextCameraPos;
	Collider* next_cam_limit;
public:
	~Door();
	virtual void action(int x, int y);
	Door(float px, float py, float cx, float cy);
	void travel();
};