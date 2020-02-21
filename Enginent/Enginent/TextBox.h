#pragma once

#include <string>
#include <vector>
#include "UIObject.h"
#include "TextObject.h"
#include "ScriptManager.h"

struct Dialogue
{
	std::string name;
	std::string dialogue;
	Dialogue(std::string n, std::string d)
	{
		name = n;
		dialogue = d;
	}
	Dialogue()
	{
		name = " ";
		dialogue = " ";
	};
};

class TextBox : public UIObject {
	private:
		UIObject* background;
		TextObject* dialogue;
		TextObject* name;
		SDL_Color textColor = {255, 255, 255, 0};
		SDL_Color nameColor = {0, 0, 0, 0 };
		bool display = true;
		int dialogueSize = 0;
		static TextBox* _instance;
		std::vector<Dialogue> dialogues;
	protected:
		TextBox();
	public:
		static TextBox* GetInstance();
		~TextBox();
		void setDialogue_vec(std::vector<Dialogue>);
		void setText(Dialogue);
		void setTextColor(SDL_Color colorN, SDL_Color colorS) { nameColor = colorN; textColor = colorS; }
		void Render();
		void setTextboxDisplay(bool b);
		void clickLeft(glm::vec3);
		void SetTextboxPosition(glm::vec3);
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