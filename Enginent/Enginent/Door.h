#pragma once

#include "InteractObj.h"
#include "Collider.h"

class Door : public InteractableObj {
protected:
	IneractTypeList interactType = DOOR;

	// new settings
	int next_room;
	int door_no;
	glm::vec3 doorLevelPosition;

	// old settings
	glm::vec3 nextPlayerPos;
	Collider* next_cam_limit;
	InteractableObj* item_to_unlock;
	bool open;
public:
	~Door();
	Door(float next_playerx, float next_playery, Collider* lim, InteractableObj* item_to_unlock);
	Door(int, int);
	void SetDoorLevelPosition(glm::vec3);
	void action();
	void Unlock(InteractableObj* item);
};