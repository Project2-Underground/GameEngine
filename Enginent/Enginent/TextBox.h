#pragma once

#include <string>
#include <vector>
#include "UIObject.h"
#include "TextObject.h"
#include "ScriptManager.h"


class TextBox : public UIObject {
	private:
		UIObject* background;
		TextObject* dialogue;
		TextObject* name;
		SDL_Color textColor = {255, 255, 255, 0};
		SDL_Color nameColor = {0, 0, 0, 0 };
		bool display;
		static TextBox* _instance;
		ScriptManager* scriptManager;
	protected:
		TextBox();
	public:
		static TextBox* GetInstance();
		~TextBox();
		void setText(std::string);
		void setTextColor(SDL_Color colorN, SDL_Color colorS) { nameColor = colorN; textColor = colorS; }
		void Render();
		void setTextboxDisplay(bool b);
		void clickLeft(glm::vec3);
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