#pragma once

#include "DrawableObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
class CombineObject :public DrawableObject
{
	glm::vec3 color;
	glm::vec3 color2;

public:
	CombineObject();
	~CombineObject();
	void SetColor(float r, float g, float b);
	void SetColor2(float r, float g, float b);
	void Render(glm::mat4 globalModelTransform);
};
