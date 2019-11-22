#pragma once

#include "ImageObject.h"
#include "Collider.h"
#include "TextObject.h"
#include "SDL.h"
#include "Inventory.h"
#include "SoundManager.h"

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
	void SetNextPosition(glm::vec3);
	void SetNextPosition(float x, float y);
	void SetTarget(InteractableObj* target);
	void SetCollder(Collider* n_col);
	void SetWalkLimit(Collider* limit);
	void setDialogue(string dialogue);
	void StopWalking();
	void CheckWalkLimit();

	Collider* col;
	TextObject* createDialogueText();
	Inventory* inventory;
private:
	Collider* walkLimit;
	InteractableObj* target;
	glm::vec3 next_position;
	SDL_Color dialogueColor;
	TextObject * dialogueText;
	string dialogue;
};

