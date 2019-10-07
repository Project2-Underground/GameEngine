#pragma once 

#include "ImageObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class UIObject : public ImageObject 
{
public:
	void Render(glm::mat4 globalModelTransform) {};
	void Render();
};