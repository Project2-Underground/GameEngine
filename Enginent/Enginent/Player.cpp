#include "Player.h"
#include "SquareMeshVbo.h"
#include "Game.h"
#include "Door.h"
#include <iostream>

Player::Player()
{
	walkLimit = nullptr;
	target = nullptr;
	next_position = glm::vec3(this->pos);
	anim = new Animator();

	Animation* move = new Animation("Move", "Texture/Character/Elias_walk.png");
	Animation* idle = new Animation("Idle", "Texture/Character/Elias_idle.png");
	Animation* pickup = new Animation("Pickup", "Texture/Character/Elias_pick.png", true);
	move->SetFrame(4);
	move->SetFramePeriod(0.25f);

	idle->SetFrame(1);
	idle->SetFramePeriod(0.0f);
	
	pickup->SetFrame(3);
	pickup->SetFramePeriod(0.25f);

	anim->AddAnimation(idle);
	anim->AddAnimation(move);
	anim->AddAnimation(pickup);
	anim->SetDefaultAnimation("Idle");

	walk = false;
	faceLeft = true;
	dialogueText = new TextObject();
	dialogueColor.r = 255;
	dialogueColor.b = 255;
	dialogueColor.g = 255;
	dialogueColor.a = 0;
	dialogue = " ";

	// inventory
	const int inventoryNum = 5;
	float start_x = -550;
	float start_y = -300;
	float space = 20;
	int size = 100;

	glm::vec3 inventoryBoxPos[inventoryNum] = { glm::vec3(start_x,start_y,1), glm::vec3(start_x + size + space,start_y,1), glm::vec3(start_x + (size + space) * 2,start_y,1), glm::vec3(start_x + (size + space) * 3  ,start_y,1), glm::vec3(start_x + (size + space) * 4  ,start_y,1) };
	inventory = new Inventory(inventoryNum, inventoryBoxPos, 100);

	for (int i = 0; i < inventoryNum; i++)
		Game::GetInstance()->AddUI(inventory->GetInventoryBox(i));
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

}

void Player::Move()
{
	if (this->pos.x > next_position.x)
	{
		if (pos.x - next_position.x > WALK_SPEED)
		{
			if (!faceLeft)
			{
				this->SetScale(-1, 1);
				faceLeft = true;
			}
			this->Translate(glm::vec3(-WALK_SPEED, 0, 0));
		}
		else
		{
			StopWalking();
		}
	}
	else if (this->pos.x < next_position.x)
	{
		if (next_position.x - pos.x > WALK_SPEED)
		{
			if (faceLeft)
			{
				this->SetScale(-1, 1);
				faceLeft = false;
			}
			this->Translate(glm::vec3(WALK_SPEED, 0, 0));
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
	if (target != nullptr) {
		target->action();
		target = nullptr;
	}
}

void Player::CheckWalkLimit() {
	if (walkLimit != nullptr) {
		// right limit
		if (pos.x + size.x * 0.5 > walkLimit->getMaxBound().x) {
			pos.x = walkLimit->getMaxBound().x - size.x * 0.5;
			StopWalking();
		}
		// left limit
		else if (pos.x - size.x * 0.5 < walkLimit->getMinBound().x) {
			pos.x = walkLimit->getMinBound().x + size.x * 0.5;
			StopWalking();
		}
	}
}

void Player::SetTarget(InteractableObj* target) {
	if (display)
	{
		this->target = target;
		this->next_position = target->getPos();
		SoundManager::GetInstance()->playSound("Walking");
		walk = true;
		setDialogue(" ");
		if (!(anim->currentAnimation->animationName == "Move"))
			anim->Play("Move", true);
	}

}

void Player::SetNextPosition(float x, float y)
{
	if (display)
	{
		next_position = glm::vec3(x, y, 1);
		walk = true;
		SoundManager::GetInstance()->playSound("Walking");
		setDialogue(" ");
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

	SoundManager::GetInstance()->playSound("Walking");
	walk = true;
	setDialogue(" ");
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

void Player::setDialogue(string dialogue)
{
	this->dialogue = dialogue;
}

Player::~Player() {
	if (col != nullptr)
		delete col;
	if (inventory != nullptr)
		delete inventory;
	if (dialogueText != nullptr)
		delete dialogueText;
}