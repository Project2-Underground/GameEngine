#include "InteractObj.h"
#include "Game.h"

InteractableObj::InteractableObj(IneractTypeList type, vector<std::string>* s) {
	dialogue = s;
	interactType = type;
}

InteractableObj::InteractableObj(IneractTypeList type, std::string s)
{
	interactType = type;
	dialogue->push_back(s);
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

void InteractableObj::SetDialogue(vector<std::string>* s) {
	dialogue = s;
}

void InteractableObj::action(int x, int y) {
	Game::GetInstance()->getPlayer()->setTarget(x, y);

	if (dialogue != nullptr)
	{
		Game::GetInstance()->getPlayer()->setDialogue((*dialogue)[currDialogue]);
		currDialogue++;
		if (currDialogue >= dialogue->size())
		{
			currDialogue = 0;
		}
	}
	//ANIMATION
	//UPDATE TEXT
}

void InteractableObj::checkCollider(int x, int y) {
	if (this->col->isClicked(x, y))
	{
		action(x, y);
	}
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
	if (dialogue)
		delete dialogue;
}