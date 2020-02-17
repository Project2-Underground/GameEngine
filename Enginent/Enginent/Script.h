#pragma once
#include <pugixml.hpp>
#include <string>
#include <map>
#include "TextBox.h"

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

class Script {
	std::map<std::string, std::vector<Dialogue>> script;
public:
	Script();
	void LoadScript();
	std::vector<Dialogue> GetDialogue();
};