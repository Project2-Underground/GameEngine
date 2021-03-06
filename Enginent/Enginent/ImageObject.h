#pragma once

#include "DrawableObject.h"
#include "Collider.h"
#include "Animator.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>

class ImageObject :public DrawableObject
{
protected:
	unsigned int texture;
	string texturePath;
	GLfloat texData[8];
	bool display;
	bool triggerAnim;
public:
	Collider* col;
	Animator* anim;

	ImageObject();
	~ImageObject();
	virtual void SetTexture(string path);
	void SetTexturePath(string path);
	virtual void SetTexture(unsigned int texture);
	void SetPosition(glm::vec3 newPosition);
	void InitAnimator() { anim = new Animator(); }
	void Appear(bool b);
	unsigned int GetTexture();
	string GetTexturePath();
	bool IsDisplay() { return display; }
	void SetDisplay(bool d) { display = d; }
	virtual void Render(glm::mat4 globalModelTransform);
	virtual void Init(float sizex, float sizey, glm::vec3 position);
	void SetCollder(Collider* n_col);
	void Update() { if (anim) anim->Update(); }
};