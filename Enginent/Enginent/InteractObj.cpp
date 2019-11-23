#include "InteractObj.h"
#include "Game.h"

InteractableObj::InteractableObj(IneractTypeList type, vector<std::string> s) {
	dialogue = s;
	interactType = type;
}

InteractableObj::InteractableObj(IneractTypeList type, std::string s)
{
	interactType = type;
	dialogue.push_back(s);
}

InteractableObj::InteractableObj(IneractTypeList type) {
	interactType = type;
}

void InteractableObj::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
	/*std::cout << "collider min bound: " << col->getMinBound().x << ", " << col->getMinBound().y << std::endl;
	std::cout << "collider max bound: " << col->getMaxBound().x << ", " << col->getMaxBound().y << std::endl;*/
}

void InteractableObj::SetDialogue(vector<std::string> s) {
	dialogue = s;
}

void InteractableObj::action() {
	if (dialogue.size() > 0)
	{
		Game::GetInstance()->getPlayer()->setDialogue(dialogue[currDialogue]);
		currDialogue = (++currDialogue) % dialogue.size();
	}
	//ANIMATION
	//SOUND
	if (interact_sound != "")
	{
		SoundManager::GetInstance()->playSFX(interact_sound);
	}
	//UPDATE TEXT
}

bool InteractableObj::checkCollider(int x, int y) {
	return this->col->isClicked(x, y);
	/*if (this->col->isClicked(x, y))
	{
		action(x, y);
	}*/
}
bool InteractableObj::checkPointing(int x, int y)
{
	if (this->col->isClicked(x, y))
	{
		return true;
	}
	return false;
}

InteractableObj::~InteractableObj() {
	if (col)
		delete col;
}

void InteractableObj::SetSound(std::string s)
{
	interact_sound = s;
}