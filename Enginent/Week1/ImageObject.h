#pragma once

#include "DrawableObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
class ImageObject :public DrawableObject
{
	unsigned int texture;

public:
	ImageObject();
	~ImageObject();
	void SetTexture(string path);
	void Render(glm::mat4 globalModelTransform);
};
