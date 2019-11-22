#pragma once

#include "InteractObj.h"
#include "Collider.h"

class Door : public InteractableObj {
protected:
	IneractTypeList interactType = DOOR;
	glm::vec3 nextPlayerPos;
	Collider* next_cam_limit;
	InteractableObj* item_to_unlock;
	bool open;
public:
	~Door();
	Door(float next_playerx, float next_playery, Collider* lim, InteractableObj* item_to_unlock);
	void action();
	void Unlock(InteractableObj* item);
};