#pragma once

#include "ImageObject.h"
#include "Collider.h"
#include "TextObject.h"
#include "SDL.h"
#include "Inventory.h"
#include "SoundManager.h"

#define WALK_SPEED 3
#define ACTION_DISTANCE 10.0f

class Player : public ImageObject
{
public:
	bool walk;
	bool faceLeft;

	Player();
	~Player();

	void Update();

	void SetNextPosition(glm::vec3);
	void SetNextPosition(float x, float y);
	void SetCollder(Collider* n_col);
	void SetWalkLimit(Collider* limit);
	void SetDialogue(string dialogue);

	void Move();
	void StopWalking();

	void CheckTarget(InteractableObj* target);
	void CheckWalkLimit();

	Collider* col;
	TextObject* createDialogueText();
	TextObject* dialogueText;
	Inventory* inventory;
	glm::vec3 next_position;
private:
	Collider* walkLimit;
	InteractableObj* target;
	SDL_Color dialogueColor;
	string dialogue;
};

