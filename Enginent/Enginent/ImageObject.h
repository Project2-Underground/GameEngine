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
	void SetPosition(glm::vec3 newPosition);
	unsigned int GetTexture();
	bool IsDisplay() { return display; }
	void SetDisplay(bool d) { display = d; }
	virtual void Render(glm::mat4 globalModelTransform);
	virtual void Init(float sizex, float sizey, glm::vec3 position);
	void SetCollder(Collider* n_col);
};
