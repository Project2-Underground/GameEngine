#include "InteractObj.h"
#include "Game.h"

InteractableObj::InteractableObj(vector<std::string>* s) {
	dialogue = s;
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