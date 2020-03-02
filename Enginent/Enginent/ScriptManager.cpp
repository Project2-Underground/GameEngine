#include "ScriptManager.h"

Dialogue::Dialogue(std::string n, std::string d, Item* i)
{
	name = n;
	dialogue = d;
	item = i;
}

Dialogue::Dialogue()
{
	name = " ";
	dialogue = " ";
	item = nullptr;
};

ScriptManager* ScriptManager::_instance = nullptr;

ScriptManager::ScriptManager()
{
	pugi::xml_parse_result result = scriptDoc.load_file("save/scripts.xml" , pugi::parse_default | pugi::parse_declaration);
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
		for (pugi::xml_node_iterator dialogue = scripts->begin(); dialogue != scripts->end(); dialogue++)
		{
			std::string name = dialogue->attribute("name").as_string();
			std::string text = dialogue->attribute("text").as_string();
			Item* item = nullptr;
			if (dialogue->child("item"))
			{
				//create item
				item = new Item(dialogue->child("item").attribute("name").as_string());
				item->SetInventoryTexture(dialogue->child("item").attribute("i_texture").as_string());
				item->SetViewTexture(dialogue->child("item").attribute("v_texture").as_string());
			}
			Dialogue tmp(name, text, item);
			s->dialogue.push_back(tmp);

		}
		this->scripts[key] = s;
	}
}

Dialogue ScriptManager::GetDialogue(std::string key)
{
	Script* tmp = scripts.at(key);
	if (tmp == nullptr)
	{
		std::cout << "Cannot find dialogue " << key << std::endl;
		return Dialogue();
	}

	int index = tmp->currIndex;
	tmp->currIndex++;
	if (tmp->currIndex >= tmp->dialogue.size())
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