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

	anim->AddAnimation("Idle", "Texture/Character/Elias_idle.png",4,0.25f);
	anim->AddAnimation("Move", "Texture/Character/Elias_walk.png",4,0.25f);
	anim->AddAnimation("Pickup", "Texture/Character/Elias_pick.png",4,0.25f,true);
	anim->AddAnimation("Panic", "Texture/Character/Elias_pick.png",3,0.25f,true);
	anim->Play("Idle", true);

	walk = false;
	faceLeft = false;
}

void Player::Update()
{
	if (display)
	{
		Move();
		if (!walk)
		{
			if (anim->currentAnimation->animationName != "Idle")
				anim->Play("Idle", true);
		}
	}
	anim->Update();

}

void Player::Move()
{
	float walk_sp = WALK_SPEED * (float)(1 / TimeSystem::instance()->GetTimeBetweenFrame());
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
	col->Update();
	CheckWalkLimit();
}

void Player::StopWalking() {
	walk = false;
	next_position = getPos();
	SoundManager::GetInstance()->stop(SFX, "Walking");
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
	float distance = abs(target->getPos().x - pos.x) - abs(target->getSize().x * 0.5f);
	//std::cout << distance << std::endl;
	if (distance <= ACTION_DISTANCE) {
		target->action();
		StopWalking();
	}
	else {
		Dialogue d("Elias", "It's too far away");
		TextBox::GetInstance()->setText(d);
		TextBox::GetInstance()->SetDisplay(true);
		StopWalking();
	}
}

void Player::SetNextPosition(float x, float y)
{
	if (walk == false)
		SoundManager::GetInstance()->playSound(SFX, "Walking", true);
	next_position = glm::vec3(x, y, 1);
	walk = true;
	if (!(anim->currentAnimation->animationName == "Move"))
		anim->Play("Move", true);
}

void Player::SetNextPosition(glm::vec3 realPos)
{
	next_position = realPos;
	if (walk == false)
		SoundManager::GetInstance()->playSound(SFX, "Walking", true);
	walk = true;
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

Player::~Player() {
	if (col != nullptr)
		delete col;
}