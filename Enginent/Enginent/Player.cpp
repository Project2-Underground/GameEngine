#include "Player.h"
#include "SquareMeshVbo.h"
#include "Game.h"
#include "TimeSystem.h"
#include <iostream>

Player::Player()
{
	walkLimit = nullptr;
	next_position = glm::vec3(this->pos);
	anim = new Animator();

	Animation* move = new Animation("Move", "Texture/Character/Elias_walk.png");
	Animation* idle = new Animation("Idle", "Texture/Character/Elias_idle.png");
	Animation* pickup = new Animation("Pickup", "Texture/Character/Elias_pick.png", true);
	Animation* panic = new Animation("Panic", "Texture/Character/Elias_panic.png", true);
	move->SetFrame(4);
	move->SetFramePeriod(0.25f);

	idle->SetFrame(2);
	idle->SetFramePeriod(0.25f);
	
	pickup->SetFrame(4);
	pickup->SetFramePeriod(0.25f);

	panic->SetFrame(3);
	panic->SetFramePeriod(0.25f);

	anim->AddAnimation(idle);
	anim->AddAnimation(move);
	anim->AddAnimation(pickup);
	anim->SetDefaultAnimation("Idle");
	anim->Play("Idle", true);

	walk = false;
	faceLeft = false;
	dialogueText = new TextObject();
	dialogueColor.r = 255;
	dialogueColor.b = 255;
	dialogueColor.g = 255;
	dialogueColor.a = 0;
	dialogue = " ";
}

void Player::Update()
{
	if (display)
	{
		Move();
		if (!walk)
		{
			anim->Play("Idle", true);
			SoundManager::GetInstance()->stop("Walking");
			dialogueText->loadText(dialogue, dialogueColor, 18);
			//dialogueText->SetPosition(glm::vec3(0.0f, -220.0f, 1.0f));
		}
	}
	anim->Update();

}

void Player::Move()
{
	float walk_sp = WALK_SPEED * (1/TimeSystem::instance()->GetTimeBetweenFrame());
	// walk left
	if (this->pos.x > next_position.x)
	{
		if (pos.x - next_position.x > walk_sp)
		{
			if (!faceLeft)
			{
				this->SetScale(-1, 1);
				faceLeft = true;
			}
			this->Translate(glm::vec3(-walk_sp, 0, 0));
		}
		else
		{
			StopWalking();
		}
	}
	// walk right
	else if (this->pos.x < next_position.x)
	{
		if (next_position.x - pos.x > walk_sp)
		{
			if (faceLeft)
			{
				this->SetScale(-1, 1);
				faceLeft = false;
			}
			this->Translate(glm::vec3(walk_sp, 0, 0));
		}
		else
		{
			StopWalking();
		}
	}
	CheckWalkLimit();
	col->Update();
}

void Player::StopWalking() {
	walk = false;
	next_position = getPos();
}

void Player::CheckWalkLimit() {
	if (walkLimit != nullptr) {
		// right limit
		if (pos.x + size.x * -0.5 >= walkLimit->getMaxBound().x) {
			pos.x = walkLimit->getMaxBound().x - size.x * -0.5f;
			StopWalking();
		}
		// left limit
		else if (pos.x - size.x * 0.5 <= walkLimit->getMinBound().x) {
			pos.x = walkLimit->getMinBound().x + size.x * 0.5f;
			StopWalking();
		}
	}
}

void Player::CheckTarget(InteractableObj* target) {
	float distance = (abs(target->getPos().x - pos.x) - (abs(size.x) * 0.5f + abs(target->getSize().x) * 0.5f));
	if (distance <= ACTION_DISTANCE) {
		target->action();
		StopWalking();
	}
}

void Player::SetNextPosition(float x, float y)
{
	if (walk == false)
	{
		SoundManager::GetInstance()->playSound("Walking", true);
	}
	if (display)
	{
		next_position = glm::vec3(x, y, 1);
		walk = true;
		SetDialogue(" ");
		if (!(anim->currentAnimation->animationName == "Move"))
			anim->Play("Move", true);
	}
}

void Player::SetNextPosition(glm::vec3 realPos)
{
	if (this->display)
	{
		next_position = realPos;
	}
	if (walk == false)
	{
		SoundManager::GetInstance()->playSound("Walking", true);
	}
	walk = true;
	SetDialogue(" ");
	if (!(anim->currentAnimation->animationName == "Move"))
		anim->Play("Move", true);
}

void Player::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
}

void Player::SetWalkLimit(Collider* limit) {
	walkLimit = limit;
}

TextObject* Player::createDialogueText()
{
	dialogueText->loadText(dialogue, dialogueColor, 18);
	dialogueText->SetPosition(glm::vec3(0.0f, -220.0f, 1.0f));
	return dialogueText;
}

void Player::SetDialogue(string dialogue)
{
	this->dialogue = dialogue;
}

Player::~Player() {
	if (col != nullptr)
		delete col;
	if (dialogueText != nullptr)
		delete dialogueText;
}