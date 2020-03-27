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
	int chatIndex;
	Item* item;
	s_Dialogue(std::string n, std::string d, Item* item, std::string chatName, int chatIndex);
	s_Dialogue();
};

struct Dialogue {
	std::vector<s_Dialogue> dialogue;
	std::string choice;
};

struct Script {
	std::vector<Dialogue> script;
	int loopIndex;
	int currIndex;
	Script(int index)
	{
		loopIndex = index;
		currIndex = 0;
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