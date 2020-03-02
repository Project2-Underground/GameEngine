#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <pugixml.hpp>
#include "Item.h"

struct Dialogue
{
	std::string name;
	std::string dialogue;
	Item* item;
	Dialogue(std::string n, std::string d, Item* item);
	Dialogue();
};

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

struct Script {
	std::vector<Dialogue> dialogue;
	int loopIndex;
	int currIndex;
	Script(int index)
	{
		this->loopIndex = index;
		this->currIndex = 0;
	}
};

class ScriptManager {
	static ScriptManager* _instance;
	std::map<std::string, Script*> scripts;
	pugi::xml_document scriptDoc;
protected:
	ScriptManager();
public:
	~ScriptManager();
	static ScriptManager* GetInstance();
	void LoadScript();
	Dialogue GetDialogue(std::string);
};