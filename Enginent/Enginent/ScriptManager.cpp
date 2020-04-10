#include "ScriptManager.h"

s_Dialogue::s_Dialogue(std::string n, std::string d, Item* i, std::string chatName, int chatIndex, std::string note)
{
	name = n;
	text = d;
	item = i;
	this->chatName = chatName;
	this->chatIndex = chatIndex;
	this->noteName = note;
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
	choiceDoc.load_file("save/choice.xml", pugi::parse_default | pugi::parse_declaration);
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
			for (pugi::xml_node_iterator dialogue = dialogue->begin(); dialogue != s_dialogue->end(); dialogue++)
			{
				std::string name = dialogue->attribute("name").as_string();
				std::string text = dialogue->attribute("text").as_string();
				Item* item = nullptr;
				int chatIndex = -1;
				std::string chatName = "";
				std::string noteName = "";
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
				if (dialogue->child("note"))
				{
					//save chat info
					noteName = dialogue->child("note").attribute("name").as_string();
				}
				s_Dialogue tmp(name, text, item, chatName, chatIndex, noteName);
				d.dialogue.push_back(tmp);
			}

			//on obj
			pugi::xml_node e_Obj = (s_dialogue)->child("enable");
			for (pugi::xml_node_iterator dialogue = s_dialogue->begin(); dialogue != s_dialogue->end(); dialogue++)
			{
			}
			//off obj
			pugi::xml_node d_Obj = (s_dialogue)->child("disable");
			for (pugi::xml_node_iterator dialogue = s_dialogue->begin(); dialogue != s_dialogue->end(); dialogue++)
			{
			}
			s->script.push_back(d);
		}
		this->scripts[key] = s;
	}

	std::cout << "load choice\n";
	pugi::xml_node choice_script = choiceDoc.child("c");
	for (pugi::xml_node_iterator choices = choice_script.begin(); choices != choice_script.end(); choices++)
	{
		std::string key = choices->attribute("key").as_string();
		std::vector<Choice>* v_choice = new std::vector<Choice>();

		pugi::xml_node s_choice = (choices)->child("choice");
		for (pugi::xml_node_iterator s_choice = choices->begin(); s_choice != choices->end(); s_choice++)
		{
			//get choice
			std::string text, next_d;
			text = s_choice->attribute("text").as_string();
			next_d = s_choice->attribute("nextDialogue").as_string();
			v_choice->push_back(Choice(text, next_d));
		}
		this->choices[key] = v_choice;
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

std::vector<Choice>* ScriptManager::GetChoice(std::string key)
{
	std::vector<Choice>* tmp = choices[key];
	if (tmp == nullptr)
	{
		std::cout << "Cannot find dialogue " << key << std::endl;
	}
	return tmp;
}

ScriptManager::~ScriptManager()
{
	for (std::map<std::string, Script*>::iterator itr = scripts.begin(); itr != scripts.end(); ++itr)
	{
		delete itr->second;
	}
	for (std::map<std::string, std::vector<Choice>*>::iterator itr = choices.begin(); itr != choices.end(); ++itr)
	{
		delete itr->second;
	}
}