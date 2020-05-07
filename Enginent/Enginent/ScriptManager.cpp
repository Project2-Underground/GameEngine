#include "ScriptManager.h"
#include "Game.h"

s_Dialogue::s_Dialogue(std::string n, std::string d, std::string item, std::string npc, std::string chatName, int chatIndex, std::string note, std::string puzzle, std::string anim)
{
	name = n;
	text = d;
	itemName = item;
	NPCName = npc;
	this->chatName = chatName;
	this->chatIndex = chatIndex;
	this->noteName = note;
	this->puzzleName = puzzle;
	this->animName = anim;
}

s_Dialogue::s_Dialogue()
{
	name = "";
	text = "";
	itemName = "";
	NPCName = "";
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
			int changeNameNum, itemDisplayNum;
			if (s_dialogue->attribute("change"))
				changeNameNum = s_dialogue->attribute("change").as_int();
			else
				changeNameNum = 0;
			if (s_dialogue->attribute("display"))
				itemDisplayNum = s_dialogue->attribute("display").as_int();
			else
				itemDisplayNum = 0;
			pugi::xml_node_iterator dialogue = s_dialogue->begin();
			////pugi::xml_node dialogue = (s_dialogue)->child("Dialogue");
			for (int i = 0; i < changeNameNum; i++)
			{
				std::string objectName = dialogue->attribute("name").as_string();
				std::string newName = dialogue->attribute("new").as_string();
				std::string newAfter = dialogue->attribute("new_a").as_string();
				ChangeName ch;
				ch.d_after = newAfter;
				ch.d_before = newName;
				d.changeNameObj[objectName] = ch;
				dialogue++;
			}
			for (int j = 0; j < itemDisplayNum; j++)
			{
				std::string objectName = dialogue->attribute("name").as_string();
				bool b_display = dialogue->attribute("display").as_bool();
				bool c_pos = dialogue->attribute("changePos").as_bool();
				float x = dialogue->attribute("posX").as_float();
				float y = dialogue->attribute("posY").as_float();
				d.displayObj.push_back(diaplayAfterAction(objectName, b_display, c_pos, x, y));
				dialogue++;
			}
			while (dialogue != s_dialogue->end())
			{
				std::string name = dialogue->attribute("name").as_string();
				std::string text = dialogue->attribute("text").as_string();
				//Item* item = nullptr;
				std::string itemName = "";
				std::string NPCName = "";
				int chatIndex = -1;
				std::string chatName = "";
				std::string noteName = "";
				std::string animName = "";
				std::string puzzleName = "";
				if (dialogue->child("item"))
				{
					itemName = dialogue->child("item").attribute("name").as_string();
					NPCName = dialogue->child("item").attribute("NPC").as_string();
					//create item
					/*item = new Item(dialogue->child("item").attribute("name").as_string());
					item->SetInventoryTexture(dialogue->child("item").attribute("i_texture").as_string());
					item->SetViewTexture(dialogue->child("item").attribute("v_texture").as_string());*/
				}
				if (dialogue->child("chat"))
				{
					chatName = dialogue->child("chat").attribute("name").as_string();
					chatIndex = dialogue->child("chat").attribute("index").as_int();
				}
				if (dialogue->child("note"))
				{
					noteName = dialogue->child("note").attribute("name").as_string();
				}
				if (dialogue->child("animation"))
				{
					animName = dialogue->child("animation").attribute("name").as_string();
				}
				if (dialogue->child("puzzle"))
				{
					puzzleName = dialogue->child("puzzle").attribute("name").as_string();
				}
				s_Dialogue tmp(name, text, itemName, NPCName, chatName, chatIndex, noteName, puzzleName, animName);
				d.dialogue.push_back(tmp);
				dialogue++;
			}

			//on obj
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
			std::string text = "", next_d = "", puzzle = "";
			text = s_choice->attribute("text").as_string();
			if(s_choice->attribute("nextDialogue"))
				next_d = s_choice->attribute("nextDialogue").as_string();
			if (s_choice->attribute("puzzle"))
			{
				puzzle = s_choice->attribute("puzzle").as_string();
			}
			v_choice->push_back(Choice(text, next_d, puzzle));
		}
		this->choices[key] = v_choice;
	}
}

Dialogue* ScriptManager::GetDialogue(std::string key, bool talk)
{
	Script* tmp = scripts[key];
	if (tmp == nullptr)
	{
		std::cout << "Cannot find dialogue " << key << std::endl;
		return nullptr;
	}

	int index = tmp->currIndex;
	tmp->currIndex++;
	if (index == -1)
	{
		index = rand() % tmp->script.size() - 1;
	}
	else if (tmp->currIndex >= tmp->script.size())
	{
		tmp->currIndex = tmp->loopIndex;
	}
	if (talk && tmp->currIndex == 0)
	{
		tmp->currIndex = 1;
	}
	return &(tmp->script[index]);
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