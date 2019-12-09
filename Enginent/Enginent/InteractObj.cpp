#include "InteractObj.h"
#include "Game.h"

InteractableObj::InteractableObj(vector<std::string> s) {
	interactType = NORMAL;
	dialogue = s;
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
		((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer()->setDialogue(dialogue[currDialogue]);
		currDialogue = (++currDialogue) % dialogue.size();
	}
	//ANIMATION
	//UPDATE TEXT
}

bool InteractableObj::CheckCollider(int x, int y) {
	return this->col->isClicked(x, y);
	/*if (this->col->isClicked(x, y))
	{
		action(x, y);
	}*/
}
bool InteractableObj::CheckPointing(int x, int y)
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

bool InteractableObj::operator==(const InteractableObj& obj) {
	if (this->object_name == obj.object_name) {
		return true;
	}
	return false;
}