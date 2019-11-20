#pragma once

#include "DrawableObject.h"
#include "Collider.h"
#include "Animator.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class ImageObject :public DrawableObject
{
protected:
	unsigned int texture;
	string texturePath;
	GLfloat texData[8];
	bool display;
public:
	Collider* col;
	Animator* anim;

	ImageObject();
	~ImageObject();
	void SetTexture(string path);
	void SetTexture(unsigned int texture);
	unsigned int GetTexture();
	virtual void Render(glm::mat4 globalModelTransform);
	void SetCollder(Collider* n_col);
	void setDisplay(bool);
};
