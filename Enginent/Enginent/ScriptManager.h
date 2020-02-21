#pragma once
#include <string>
#include <vector>
#include <map>

struct Dialogue
{
	std::string name;
	std::string dialogue;
	Dialogue(std::string n, std::string d);
	Dialogue();
};

struct Script {
	std::vector<Dialogue> dialogue;
	int type;
	int loopIndex;
	int currIndex;
	Script(int type, int index)
	{
		this->type = type;
		this->loopIndex = index;
		this->currIndex = 0;
	}
};

class ScriptManager {
	static ScriptManager* _instance;
	std::map<std::string, Script>* scripts;
protected:
	ScriptManager();
public:
	~ScriptManager();
	ScriptManager* GetInstance();
	void LoadScript(std::string);
	std::vector<Dialogue> GetDialogue();
};