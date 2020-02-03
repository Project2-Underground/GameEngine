#pragma once

#include <string>
#include <vector>
#include "UIObject.h"
#include "TextObject.h"

class TextBox : public UIObject {
	private:
		ImageObject* background;
		TextObject* dialogue;
		TextObject* name;
		SDL_Color textColor = {0, 0, 0, 0};
		bool display = true;
	public:
		TextBox(int sizeX, int sizeY, int posX, int posY);
		~TextBox();
		void setText(std::string name, std::string text);
		void setTextColor(SDL_Color color) { textColor = color; }
		void Render();
		void toggleDisplay();
};

class ChoiceBox : UIObject {
	int choiceID;
	std::string text;
	ImageObject* background;
public:
	ChoiceBox();
	void setText(std::string text);
};

class ChoiceUI {
	std::vector<ChoiceBox*> list;
public:
	ChoiceUI();
	void addChoice(ChoiceBox* c);
};