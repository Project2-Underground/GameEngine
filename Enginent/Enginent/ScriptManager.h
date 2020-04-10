#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <pugixml.hpp>
#include "Item.h"
#include "ImageObject.h"

struct Choice {
	std::string text;
	std::string nextScript;
	Choice(std::string& t, std::string& next)
	{
		text = t;
		nextScript = next;
	}
	Choice() {};
};

struct s_Dialogue
{
	std::string name;
	std::string text;
	std::string chatName;
	std::string noteName;
	int chatIndex;
	Item* item;
	s_Dialogue(std::string n, std::string d, Item* item, std::string chatName, int chatIndex, std::string note);
	s_Dialogue();
};

struct Dialogue {
	std::vector<s_Dialogue> dialogue;
	std::vector<std::string>* nameObj_On;
	std::vector<std::string>* nameObj_Off;
	std::string choice;
	Dialogue() {
		choice = "";
		nameObj_Off = nullptr;
		nameObj_On = nullptr;
	}
	~Dialogue() {
		if (nameObj_Off != nullptr)
		{
			delete nameObj_Off;
		}
		if (nameObj_On != nullptr)
		{
			delete nameObj_On;
		}
	}
};

struct Script {
	std::vector<Dialogue> script;
	int loopIndex = 0;
	int currIndex = 0;
	Script(int index)
	{
		loopIndex = index;
	}
};

class ScriptManager {
	static ScriptManager* _instance;
	std::map<std::string, Script*> scripts;
	std::map<std::string, std::vector<Choice>*> choices;
	pugi::xml_document scriptDoc;
	pugi::xml_document choiceDoc;
	Dialogue* displayText;
protected:
	ScriptManager();
public:
	~ScriptManager();
	static ScriptManager* GetInstance();
	void LoadScript();
	Dialogue GetDialogue(std::string);
	std::vector<Choice>* GetChoice(std::string);
};