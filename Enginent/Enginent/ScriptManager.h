#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <pugixml.hpp>
#include "Item.h"

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
	Item* item;
	s_Dialogue(std::string n, std::string d, Item* item, );
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
	pugi::xml_document scriptDoc;
	Dialogue* displayText;
protected:
	ScriptManager();
public:
	~ScriptManager();
	static ScriptManager* GetInstance();
	void LoadScript();
	Dialogue GetDialogue(std::string);
};