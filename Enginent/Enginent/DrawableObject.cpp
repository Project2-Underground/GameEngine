
#include "DrawableObject.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"



glm::mat4 DrawableObject::getTransform()
{
	glm::mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0));
	transform = glm::scale(transform, glm::vec3(size.x, size.y, 1));
	transform = glm::rotate(transform, rot, glm::vec3(0, 0, 1));
	return transform;
}

bool DrawableObject::operator<(const DrawableObject& obj)const {
	return (this->layer < obj.layer);
}

DrawableObject::DrawableObject()
{
	pos = glm::vec3(0.0, 0.0, 0.0);
	size = glm::vec3(1.0, 1.0, 1.0);
	rot = 0;
}


DrawableObject::~DrawableObject()
{
}

glm::vec3 DrawableObject::getPos() {
	return pos;
}

glm::vec3 DrawableObject::getSize() {
	return size;
}

void DrawableObject::SetSize(float sizeX, float sizeY)
{
	size = glm::vec3(sizeX, sizeY, 1);
}

void DrawableObject::SetPosition(glm::vec3 newPosition)
{
	pos = newPosition;
}

void DrawableObject::Translate(glm::vec3 moveDistance)
{
	pos = pos + moveDistance;
}

void DrawableObject::SetRotate(float r)
{
	this->rot = r * 3.14f / 180.0f;
}

void DrawableObject::SetScale(float x, float y)
{
	this->size.x *= x;
	this->size.y *= y;
}

void DrawableObject::SetName(std::string name) {
	object_name = name;
}

