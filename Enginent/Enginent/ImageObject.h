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

public:
	Collider* col;
	Animator* anim;

	ImageObject();
	~ImageObject();
	void SetTexture(string path);
	unsigned int GetTexture();
	virtual void Render(glm::mat4 globalModelTransform);
	void SetCollder(Collider* n_col);
};
