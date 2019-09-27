#pragma once 

#include "ImageObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
class UIObject : public DrawableObject {
protected:
	unsigned int texture;
	string texturePath;

public:
	UIObject();
	~UIObject();
	void SetTexture(string path);
	unsigned int GetTexture();
	virtual void Render();
};