#pragma once

#include <string>
#include <vector>
#include "UIObject.h"
#include "TextObject.h"

class TextBox : UIObject {
	private:
		ImageObject* background;
		TextObject* dialogue;
		TextObject* name;
		SDL_Color textColor;
		bool display = false;
	public:
		TextBox(int sizeX, int sizeY, int posX, int posY);
		void setText(std::string name, std::string text);
		void setColor(SDL_Color color);
		void Render();
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