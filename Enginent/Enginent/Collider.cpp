#include "Collider.h"

Collider::Collider(DrawableObject* ref) {
	this->refObjectPos = ref;
	this->pos = ref->getPos();
	this->size = ref->getSize();
	this->maxBound = glm::vec3(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, pos.z);
	this->minBound = glm::vec3(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, pos.z);
}

Collider::Collider(glm::vec3 pos, glm::vec3 size) {
	this->refObjectPos = nullptr;
	this->pos = pos;
	this->size = size;
	this->maxBound = glm::vec3(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, pos.z);
	this->minBound = glm::vec3(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, pos.z);
}

Collider::Collider(glm::vec3 pos, float w, float h) {
	this->refObjectPos = nullptr;
	this->pos = pos;
	this->size = glm::vec3(w, h, 1);
	this->maxBound = glm::vec3(pos.x + w * 0.5f, pos.y + h * 0.5f, pos.z);
	this->minBound = glm::vec3(pos.x - w * 0.5f, pos.y - h * 0.5f, pos.z);
}

Collider::Collider() {
	this->refObjectPos = nullptr;
	this->pos = glm::vec3(0.0f, 0.0f, 1.0f);
	this->size = glm::vec3(1.0f, 1.0f, 1.0f);
	this->maxBound = glm::vec3(0.0f, 0.0f, 1.0f);
	this->minBound = glm::vec3(0.0f, 0.0f, 1.0f);
}

Collider::~Collider() {

}

void Collider::Update() {
	if (refObjectPos != nullptr) {
		/* update the position of the collider */
		setNewPos(refObjectPos->getPos());
	}
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
	glm::vec3 shiftDist = glm::vec3(newPos.x, newPos.y, newPos.z) - this->pos;
	this->maxBound += shiftDist;
	this->minBound += shiftDist;
	this->pos = newPos;
}

void Collider::setNewPos(float x, float y, float z) {
	/* shift min and max bound with the new pos 
	   then set the new pos
	   */
	glm::vec3 shiftDist = glm::vec3(x, y, z) - this->pos;
	this->maxBound += shiftDist;
	this->minBound += shiftDist;
	this->pos = glm::vec3(x, y, z);
}

void Collider::setNewSize(glm::vec3 newSize) {
	this->size = newSize;
	this->size.z = 1;
}

void Collider::setNewSize(float x, float y) {
	this->size = glm::vec3(x, y, 1);
}

void Collider::setNewWidth(float newW) {
	this->size.x = newW;
}

void Collider::setNewHeight(float newH) {
	this->size.y = newH;
}

bool Collider::isCollide(Collider* other) {

}
