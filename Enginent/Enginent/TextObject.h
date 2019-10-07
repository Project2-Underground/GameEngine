#pragma once

#include "DrawableObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <SDL_ttf.h>

class TextObject :public DrawableObject
{
private:
	unsigned int texture;

public:
	TextObject();
	~TextObject();
	void Render(glm::mat4 globalModelTransform);
	void update(float deltaTime);
	void loadText(string text, SDL_Color textColor, int fontSize);
	
};