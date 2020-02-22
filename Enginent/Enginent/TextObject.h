#pragma once

#include "DrawableObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <SDL_ttf.h>
#include "UIObject.h"

class TextObject :public UIObject
{
private:
	unsigned int texture;

public:
	TextObject();
	~TextObject();
	void Render();
	void update(float deltaTime);
	void loadText(string text, SDL_Color textColor, int fontSize);
	unsigned int GetTexture();
};