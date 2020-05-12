#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "UIObject.h"
#include "TextObject.h"
#include "ScriptManager.h"

constexpr auto MAX_CHAR = 80;

class ChoiceBox : UIObject {
	Choice choice;
	UIObject* background;
	TextObject* text;
	SDL_Color choiceColor = { 0, 0, 0, 0 };
public:
	ChoiceBox();
	void setChoice(Choice);
	void SetPos(glm::vec3);
	void Render();
	bool CheckClick(glm::vec3);
};

class ChoiceUI : public UIObject {
	ChoiceBox choiceList[10];
	int choiceNum;
	ScriptManager* scriptManager;
public:
	ChoiceUI();
	void setChoice(std::string key);
	void Render();
	void clickLeft(glm::vec3);
};

class TextBox : public UIObject {
	private:
		UIObject* background;
		UIObject* cutscene;
		std::vector<TextObject*> dialogue;
		std::vector<std::string> curr_dialogue;
		std::vector<std::string> show_dialogue;
		int curr_dialogueSize = 0;
		int charCount = 0;
		bool showAll = true;
		TextObject* name;
		SDL_Color textColor = {255, 255, 255, 0};
		SDL_Color nameColor = {0, 0, 0, 0 };
		static TextBox* _instance;
		ScriptManager* scriptManager;
		Dialogue* d_text;
		int d_index;
		ChoiceUI* choice_UI;
		float time = 0;
	protected:
		TextBox();
	public:
		static TextBox* GetInstance();
		~TextBox();
		void CloseBox() { display = false; }
		void setText(std::string, bool talk = false);
		void setTextColor(SDL_Color colorN, SDL_Color colorS) { nameColor = colorN; textColor = colorS; }
		void Render();
		void clickLeft(glm::vec3);
		void Update();
};