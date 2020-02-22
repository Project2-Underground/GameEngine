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
}

ScriptManager* ScriptManager::GetInstance()
{
	if (this->_instance == nullptr)
	{
		_instance = new ScriptManager();
	}
	return _instance;
}

void ScriptManager::LoadScript(std::string filename)
{

}

std::vector<Dialogue> ScriptManager::GetDialogue()
{
	std::vector<Dialogue> d;
	return d;
}

ScriptManager::~ScriptManager()
{
	delete scripts;
}