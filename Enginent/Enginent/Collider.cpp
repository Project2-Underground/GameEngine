#include "Collider.h"
#include <iostream>

Collider::Collider(DrawableObject* ref) {
	this->enable = true;
	this->refObjectPos = ref;
	this->pos = ref->getPos();
	this->pos.z = 1;
	this->size = ref->getSize();
	CalNewBound();
}

Collider::Collider() {
	this->enable = true;
	this->refObjectPos = nullptr;
	this->pos = glm::vec3(0.0f, 0.0f, 1.0f);
	this->size = glm::vec3(10.0f, 10.0f, 1.0f);
	CalNewBound();
}

Collider::~Collider() {

}

void Collider::Update() {
	/* update the position of the collider */
	setNewPos(refObjectPos->getPos());
}

glm::vec3 Collider::getMinBound() {
	return minBound;
}

glm::vec3 Collider::getMaxBound() {
	return maxBound;
}

void Collider::setRefObject(DrawableObject* ref) {
	this->refObjectPos = ref;
}

void Collider::setNewPos(glm::vec3 newPos) {
	/* shift min and max bound with the new pos
	   then set the new pos
	   */
	glm::vec3 shiftDist = glm::vec3(newPos.x, newPos.y, 1) - this->pos;
	this->maxBound += shiftDist;
	this->minBound += shiftDist;
	this->pos = newPos;
}

void Collider::setNewPos(float x, float y) {
	/* shift min and max bound with the new pos 
	   then set the new pos
	   */
	glm::vec3 shiftDist = glm::vec3(x, y, 1) - this->pos;
	this->maxBound += shiftDist;
	this->minBound += shiftDist;
	this->pos = glm::vec3(x, y, 1);
}

void Collider::setNewSize(glm::vec3 newSize) {
	this->size = newSize;
	this->size.z = 1;
	CalNewBound();
}

void Collider::setNewSize(float x, float y) {
	this->size = glm::vec3(x, y, 1);
	CalNewBound();
}

void Collider::setNewWidth(float newW) {
	this->size.x = newW;
	CalNewBound();
}

void Collider::setNewHeight(float newH) {
	this->size.y = newH;
	CalNewBound();
}

void Collider::CalNewBound() {
	this->maxBound = glm::vec3(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f * -1, pos.z);
	this->minBound = glm::vec3(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f * -1, pos.z);
}

bool Collider::isCollide(Collider* other) {
	if (enable) {
		if (this->maxBound.y <= other->getMaxBound().y && this->maxBound.y >= other->getMinBound().y &&
			this->pos.x <= other->getMaxBound().x && this->pos.x >= other->getMinBound().x) {
			return true;
		}// top
		else if (this->minBound.y <= other->getMaxBound().y && this->minBound.y >= other->getMinBound().y &&
			this->pos.x <= other->getMaxBound().x && this->pos.x >= other->getMinBound().x) {
			return true;
		}// bottom
		else if (this->minBound.x <= other->getMaxBound().x && this->minBound.x >= other->getMinBound().x &&
			this->pos.y <= other->getMaxBound().y && this->pos.y >= other->getMinBound().y) {
			return true;
		}// left
		else if (this->maxBound.x <= other->getMaxBound().x && this->maxBound.x >= other->getMinBound().x &&
			this->pos.y <= other->getMaxBound().y && this->pos.y >= other->getMinBound().y) {
			return true;
		} // right
	}
	return false;
}

bool Collider::isClicked(int x, int y) {
	if (enable) {
		if (x >= this->minBound.x && x <= this->maxBound.x &&
			y >= this->minBound.y && y <= this->maxBound.y) {
			return true;
		}
	}
	return false;
}
