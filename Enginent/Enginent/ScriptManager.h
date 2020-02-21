//#pragma once
//#include <pugixml.hpp>
//#include <string>
//#include <map>
//#include "TextBox.h"
//#include "XMLManager.h"
//
//enum ScriptType {
//	LOOP_ALL = 0,
//	LOOP_LAST,
//	LOOP_RANGE
//};
//
//class Script {
//	public
//	std::vector<Dialogue> dialogue;
//	int type;
//	int limitLoop;
//	Script(int type, int limit)
//	{
//		this->type = type;
//		this->limitLoop = limit;
//	}
//};
//
//class ScriptManager {
//	static ScriptManager* _instance;
//	std::map<std::string, Script>* scripts;
//protected:
//	ScriptManager();
//public:
//	~ScriptManager();
//	ScriptManager* GetInstance();
//	void LoadScript(std::string);
//	std::vector<Dialogue> GetDialogue();
//};