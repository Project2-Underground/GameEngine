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
	~Player();
	void Update();
	void Move();
	void Walk(int x, int y);
	void setTarget(int x, int y);
	void SetCollder(Collider* n_col);
	void setDialogue(string dialogue);
	TextObject* createDialogueText();
	Collider* col;
	Inventory* inventory;
private:
	glm::vec3 target;
	SDL_Color dialogueColor;
	TextObject * dialogueText;
	string dialogue;
};

