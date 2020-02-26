#include "ScriptManager.h"

Dialogue::Dialogue(std::string n, std::string d)
{
	name = n;
	dialogue = d;
}

Dialogue::Dialogue()
{
	name = " ";
	dialogue = " ";
};

ScriptManager* ScriptManager::_instance = nullptr;

ScriptManager::ScriptManager()
{
	std::string filename = "save/script.xml";
	scriptDoc.load_file(filename.c_str(), pugi::parse_default | pugi::parse_declaration);
	LoadScript();
}

ScriptManager* ScriptManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new ScriptManager();
	}
	return _instance;
}

void ScriptManager::LoadScript()
{

}

Dialogue ScriptManager::GetDialogue(std::string key)
{
	Script* tmp = (*scripts)[key];
	int index = tmp->currIndex;
	tmp->currIndex++;
	if (tmp->currIndex > tmp->dialogue.size())
	{
		tmp->currIndex = tmp->loopIndex;
	}
	return tmp->dialogue[index];
}

ScriptManager::~ScriptManager()
{
	for (std::map<std::string, Script*>::iterator itr = (*scripts).begin(); itr != (*scripts).end(); ++itr)
	{
		delete itr->second;
	}
	delete scripts;
}