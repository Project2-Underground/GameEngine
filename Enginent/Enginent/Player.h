#pragma once

#include "ImageObject.h"
#include "Collider.h"
#include "TextObject.h"
#include "SDL.h"
#include "Inventory.h"

#define WALK_SPEED 3

class Player : public ImageObject
{
public:
	bool walk;
	bool faceLeft;
	Player();
	void Update();
	void Move();
	void setTarget(glm::vec3);
	void setTarget(float x, float y);
	Collider* col;
	void SetCollder(Collider* n_col);
	TextObject* createDialogueText();
	void setDialogue(string dialogue);
	Inventory* inventory;
private:
	glm::vec3 target;
	SDL_Color dialogueColor;
	TextObject * dialogueText;
	string dialogue;
};

