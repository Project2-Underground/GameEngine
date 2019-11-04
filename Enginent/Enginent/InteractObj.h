#pragma once

#include "ImageObject.h"
#include "Collider.h"
#include "Game.h"

enum IneractTypeList
{
	NORMAL = 0,
	VIEW,
	PICKUP,
	TALK,
	MOVESCENE
};

class InteractableObj : public ImageObject {
	IneractTypeList interactType = NORMAL;
	vector<std::string>* dialogue;
	int currDialogue = 0;
	Collider* col;
public:
	InteractableObj() {};
	InteractableObj(vector<std::string>* s) {
		dialogue = s;
	};
	void SetCollder(Collider* n_col) {
		printf("Col create\n");
		col = n_col;
		col->setRefObject(this);
	};
	virtual void action(int x, int y) {
		Game::GetInstance()->getPlayer()->setTarget(glm::vec3(x , y , 1));
		
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

	};
	void setType(IneractTypeList newInteractType) { interactType = newInteractType; };
	IneractTypeList getType() { return interactType; };
	void checkCollider(int x, int y){ 
		if (this->col->isClicked(x, y))
		{
			action(x, y);
		}
	}
};

