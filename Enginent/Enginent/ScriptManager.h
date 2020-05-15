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
	std::string puzzleName;
	std::string roomName = "";
	std::string doorName = "";
	Choice(std::string& t, std::string& next, std::string& puzzle, std::string& room, std::string& door)
	{
		text = t;
		nextScript = next;
		puzzleName = puzzle;
		roomName = room;
		doorName = door;
	}
	Choice() {};
};

struct s_Dialogue
{
	std::string name;
	std::string text;
	std::string chatName;
	std::string noteName;
	std::string itemName;
	std::string NPCName;
	std::string animName;
	std::string puzzleName;
	std::string soundName;
	std::string Enable;
	int level = -1;
	bool en;
	std::string roomName = "";
	std::string doorName = "";
	std::string doorOpen = "";
	std::string spriteChange = "";
	std::string spriteName = "";
	unsigned int sprite;
	unsigned int CutScene;
	bool CutScenebl = 0;
	bool showItemWin;
	int chatIndex;
	s_Dialogue(std::string n, std::string d, std::string item, std::string npc, std::string chatName, int chatIndex, std::string note, std::string puzzle, std::string anim, std::string sound, bool show);
	s_Dialogue();
};

struct diaplayAfterAction {
	std::string objName;
	bool display;
	bool c_pos;
	float posX;
	float posY;
	diaplayAfterAction(std::string n, bool d, bool c, float x, float y)
	{
		objName = n;
		display = d;
		c_pos = c;
		posX = x;
		posY = y;
	}
};

struct ChangeName {
	std::string d_before;
	std::string d_after;
};

struct Dialogue {
	std::vector<s_Dialogue> dialogue;
	std::vector<diaplayAfterAction> displayObj;
	std::map<std::string, ChangeName> changeNameObj;
	std::string choice;
	int nextLevel = -1;
	Dialogue() {
		choice = "";
	}
	~Dialogue() {
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
	Dialogue* GetDialogue(std::string, bool talk = false);
	std::vector<Choice>* GetChoice(std::string);
};