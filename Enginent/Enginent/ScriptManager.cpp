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
	pugi::xml_parse_result result = scriptDoc.load_file("save/scripts.xml" , pugi::parse_default | pugi::parse_declaration);
	std::cout << result << std::endl;
	if (result)
		LoadScript();
	else
		std::cout << "Cannot load script file.\n";
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
	pugi::xml_node script = scriptDoc.child("scripts");
	for (pugi::xml_node_iterator scripts = script.begin(); scripts != script.end(); scripts++)
	{
		std::string key = scripts->attribute("key").as_string();
		Script* s = new Script(scripts->attribute("loopIndex").as_int());

		pugi::xml_node dialogue = (scripts)->child("Dialogue");
		for (pugi::xml_node_iterator d = dialogue.begin(); d != dialogue.end(); d++)
		{
			std::string name = d->attribute("name").as_string();
			std::string text = d->attribute("text").as_string();
			Dialogue tmp(name, text);
			s->dialogue.push_back(tmp);
			std::cout << name << ": " << text << std::endl;
		}
		this->scripts[key] = s;
	}
}

Dialogue ScriptManager::GetDialogue(std::string key)
{
	Script* tmp = scripts[key];
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
	for (std::map<std::string, Script*>::iterator itr = scripts.begin(); itr != scripts.end(); ++itr)
	{
		delete itr->second;
	}
}