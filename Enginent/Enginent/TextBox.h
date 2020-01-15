#pragma once

#include <string>
#include "UIObject.h"
#include "TextObject.h"

class TextBox : UIObject {
	private:
		ImageObject* background;
		TextObject* dialogue;
	public:
		TextBox(std::string name, int sizeX, int sizeY, int posX, int posY);
		void setText(std::string text);
		void Render();
};