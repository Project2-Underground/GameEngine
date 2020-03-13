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
		//bool display;
		static TextBox* _instance;
		ScriptManager* scriptManager;
		Dialogue d_text;
		int d_index;
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
	Choice choice;
	ImageObject* background;
	TextObject* text;
	SDL_Color choiceColor = { 0, 0, 0, 0 };
public:
	ChoiceBox(Choice c);
	void setText(std::string text);
	void SetPos(glm::vec3);
	void Render();
};

class ChoiceUI : public UIObject {
	std::vector<ChoiceBox*> choiceList;
	int choiceNum;
public:
	ChoiceUI();
	void addChoice(ChoiceBox* c);
	void Render();
};