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
	printf("Col create\n");
	col = n_col;
	col->setRefObject(this);
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