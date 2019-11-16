#pragma once

#include "InteractObj.h"

class Stair : public InteractableObj {
	glm::vec3 upperPos;
	glm::vec3 lowerPos;
public:
	Stair(float upperPosX, float upperPosY, float lowerPosX, float lowerPosY);
	virtual void action();
};