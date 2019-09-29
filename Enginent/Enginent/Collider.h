#pragma once

#include "Game.h"

class Collider {
	glm::vec3 pos;
	glm::vec3 size;
	glm::vec3 minBound;
	glm::vec3 maxBound;
	DrawableObject* refObjectPos;
public:
	Collider(DrawableObject* ref);
	Collider(glm::vec3 pos, float w, float h);
	Collider(glm::vec3 pos, glm::vec3 size);
	Collider();
	~Collider();

	void Update();

	glm::vec3 getMinBound();
	glm::vec3 getMaxBound();
	void setRefObject(DrawableObject* ref);
	void setNewPos(glm::vec3 newPos);
	void setNewPos(float x, float y, float z);
	void setNewSize(glm::vec3 newSize);
	void setNewSize(float x, float y);
	void setNewWidth(float newW);
	void setNewHeight(float newH);
	bool isCollide(Collider *other);
};