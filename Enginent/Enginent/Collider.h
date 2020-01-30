#pragma once
#include "glm.hpp"
#include "DrawableObject.h"

class Collider {
	glm::vec3 pos;
	glm::vec3 size;
	glm::vec3 minBound;
	glm::vec3 maxBound;
	DrawableObject* refObjectPos;
public:
	bool enable;

	Collider(DrawableObject* ref);
	Collider(glm::vec3, glm::vec3);
	Collider();
	~Collider();

	void Update();

	glm::vec3 getMinBound();
	glm::vec3 getMaxBound();
	glm::vec3 getPosition() { return pos; }
	void setRefObject(DrawableObject* ref);
	void setNewPos(glm::vec3 newPos);
	void setNewPos(float x, float y);
	void setNewSize(glm::vec3 newSize);
	void setNewSize(float x, float y);
	void setNewWidth(float newW);
	void setNewHeight(float newH);
	bool isCollide(Collider *other);
	bool isClicked(float x, float y);
	void CalNewBound();
};

bool IsMouseCollide(Collider* c, float x, float y);