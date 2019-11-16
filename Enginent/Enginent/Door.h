#pragma once

#include "InteractObj.h"

class Door : public InteractableObj {
	glm::vec3 nextPlayerPos;
	glm::vec3 nextCameraPos;
public:
	Door(float px, float py, float cx, float cy);
	void travel();
};