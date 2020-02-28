#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <pugixml.hpp>

struct Dialogue
{
	std::string name;
	std::string dialogue;
	Dialogue(std::string n, std::string d);
	Dialogue();
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