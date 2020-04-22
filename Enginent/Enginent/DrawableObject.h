#pragma once
#include "glm.hpp"
#include <vector>

using namespace std;

class DrawableObject
{
protected:
	glm::vec3 pos;
	glm::vec3 size;
	float rot;
	float test;
	float aspect;

public:
	std::string object_name;
	int layer;
	int subLayer;

	glm::mat4 getTransform();

	DrawableObject();
	~DrawableObject();
	virtual void Update() {}
	virtual void Render(glm::mat4 globalModelTransform) = 0;
	glm::vec3 getPos();
	glm::vec3 getSize();
	void SetSize(float sizeX, float sizeY);
	void SetSize(glm::vec3);
	virtual void SetPosition(glm::vec3 newPosition);
	void Translate(glm::vec3 moveDistance);
	void SetName(std::string);
	bool operator<(const DrawableObject& obj)const;
	float GetAspect() { return aspect; }

	//Rotate, Scale ???
	void SetRotate(float r);
	void SetScale(float x, float y);
};

