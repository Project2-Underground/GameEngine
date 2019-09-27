#pragma once

#include "DrawableObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
class ImageObject :public DrawableObject
{
protected:
	unsigned int texture;
	string texturePath;

public:
	ImageObject();
	~ImageObject();
	void SetTexture(string path);
	unsigned int GetTexture();
	virtual void Render(glm::mat4 globalModelTransform);
};
