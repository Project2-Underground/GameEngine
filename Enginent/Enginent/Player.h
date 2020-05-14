#pragma once

#include "ImageObject.h"
#include "Collider.h"
#include "TextObject.h"
#include "SDL.h"
#include "Inventory.h"
#include "SoundManager.h"
#include "InteractObj.h"


class Player : public InteractableObj
{
public:
	bool TriggerRouteB;
	bool TriggerRouteA;

	int walkSpeed = 250;
	const float ACTION_DISTANCE = 70.0f;
	bool walk;
	bool faceLeft;

	Player();
	~Player();

	void Update();

	void action();
	void UseItem(Item* item);

	void SetNextPosition(glm::vec3);
	void SetNextPosition(float x, float y);
	void SetCollder(Collider* n_col);
	void SetWalkLimit(Collider* limit);

	void Turn();
	void Move();
	void StopWalking();
	void AddTriggerRouteB(std::string);

	void CheckTarget(InteractableObj* target);
	//void CheckWalkLimit();

	glm::vec3 next_position;
private:
	std::string RouteB_Dialogue;
	std::vector<std::string> npcTalkRouteB;
	Collider* walkLimit;
	InteractableObj* target;
};

