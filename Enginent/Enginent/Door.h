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
	Door(float next_playerx, float next_playery, float next_camx, float next_camy);
	void SetNextCamLimit(Collider* lim);
	void travel();
};