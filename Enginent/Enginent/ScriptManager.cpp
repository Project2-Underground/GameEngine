#include "ScriptManager.h"

s_Dialogue::s_Dialogue(std::string n, std::string d, Item* i, std::string chatName, int chatIndex)
{
	name = n;
	text = d;
	item = i;
	this->chatName = chatName;
	this->chatIndex = chatIndex;
}

s_Dialogue::s_Dialogue()
{
	name = "";
	text = "";
	item = nullptr;
	chatName = "";
	chatIndex = -1;
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
		
		pugi::xml_node s_dialogue = (scripts)->child("sub");
		for (pugi::xml_node_iterator s_dialogue = scripts->begin(); s_dialogue != scripts->end(); s_dialogue++)
		{
			//get choice
			Dialogue d;
			if (s_dialogue->attribute("choice"))
			{
				d.choice = s_dialogue->attribute("choice").as_string();
			}
			else
			{
				d.choice = "";
			}

			pugi::xml_node dialogue = (s_dialogue)->child("Dialogue");
			for (pugi::xml_node_iterator dialogue = s_dialogue->begin(); dialogue != s_dialogue->end(); dialogue++)
			{
				std::string name = dialogue->attribute("name").as_string();
				std::string text = dialogue->attribute("text").as_string();
				Item* item = nullptr;
				int chatIndex = -1;
				std::string chatName = "";
				if (dialogue->child("item"))
				{
					//create item
					item = new Item(dialogue->child("item").attribute("name").as_string());
					item->SetInventoryTexture(dialogue->child("item").attribute("i_texture").as_string());
					item->SetViewTexture(dialogue->child("item").attribute("v_texture").as_string());
				}
				if (dialogue->child("chat"))
				{
					//save chat info
					chatName = dialogue->child("chat").attribute("name").as_string();
					chatIndex = dialogue->child("chat").attribute("index").as_int();
				}
				s_Dialogue tmp(name, text, item, chatName, chatIndex);
				d.dialogue.push_back(tmp);
				std::cout << name << ": " << text << std::endl;
			}
			s->script.push_back(d);
			std::cout << std::endl << "--------------------------------------" << std::endl;
		}
		this->scripts[key] = s;
	}
}

Dialogue ScriptManager::GetDialogue(std::string key)
{
	Script* tmp = scripts[key];
	if (tmp == nullptr)
	{
		std::cout << "Cannot find dialogue " << key << std::endl;
		return Dialogue();
	}

	int index = tmp->currIndex;
	tmp->currIndex++;
	if (tmp->currIndex >= tmp->script.size())
	{
		tmp->currIndex = tmp->loopIndex;
	}
	return tmp->script[index];
}

ScriptManager::~ScriptManager()
{
	for (std::map<std::string, Script*>::iterator itr = scripts.begin(); itr != scripts.end(); ++itr)
	{
		delete itr->second;
	}
}