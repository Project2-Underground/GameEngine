#include "Player.h"
#include "SquareMeshVbo.h"
#include "Game.h"
#include "TimeSystem.h"
#include "MouseInput.h"
#include <iostream>

Player::Player()
{
	object_name = "Elias";
	walkLimit = nullptr;
	next_position = glm::vec3(this->pos);
	anim = new Animator();

	anim->AddAnimation("Idle", "Texture/Character/Elias_idle.png",4,0.25f);
	anim->AddAnimation("Move", "Texture/Character/Elias_walk.png",4,0.25f);
	anim->AddAnimation("Pickup", "Texture/Character/Elias_pick.png",4,0.25f,true);
	anim->AddAnimation("Panic", "Texture/Character/Elias_pick.png",1,0.75f,true);
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

void Player::action() {
	if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
		UseItem(gs->GetInventory()->GetSelectedItem());
	}
}

void Player::UseItem(Item* item) {
	if (item) {
		if (item->name == "Puzzle6_PillMorning") {
			//std::cout << "item is used and removed from the inventory\n";
			GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
			Inventory* i = gs->GetInventory();
			TextBox::GetInstance()->setText(item->GetDialogueAfterUseWith(this->object_name));
			i->RemoveItem(item);
			i->UnselectItem();
			gs->FindItem("Puzzle6_PillBedtime_withMEAT")->ChangeDialogueAfterUsedWithObj("Puzzle6_NPC2", "Building4_med2meatmed1");
		}
	}
}

void Player::Turn() {
	SetScale(-1, 1);
	faceLeft = !faceLeft;
}

void Player::Move()
{
	//std::cout << TimeSystem::instance()->GetDT() << std::endl;
	float walk_sp = walkSpeed * TimeSystem::instance()->GetDT();
	// walk left
	if (this->pos.x > next_position.x)
	{
		if (pos.x - next_position.x > walk_sp)
		{
			if (!faceLeft)
			{
				Turn();
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
				Turn();
			}
			this->Translate(glm::vec3(walk_sp, 0, 0));
		}
		else
		{
			StopWalking();
		}
	}
	col->Update();
}

void Player::StopWalking() {
	walk = false;
	next_position = getPos();
	SoundManager::GetInstance()->stop(SFX, "Walking");
}

//void Player::CheckWalkLimit() {
//	if (walkLimit != nullptr) {
//		// right limit
//		if (pos.x + size.x * -0.5 >= walkLimit->getMaxBound().x) {
//			pos.x = walkLimit->getMaxBound().x - size.x * -0.5f;
//			StopWalking();
//		}
//		// left limit
//		else if (pos.x - size.x * 0.5 <= walkLimit->getMinBound().x) {
//			pos.x = walkLimit->getMinBound().x + size.x * 0.5f;
//			StopWalking();
//		}
//	}
//}

void Player::CheckTarget(InteractableObj* target) {
	float distance = abs(target->getPos().x - pos.x) - abs(target->getSize().x * 0.5f);
	if (distance <= ACTION_DISTANCE) {
		target->action();
		StopWalking();
		float playerPosFromTarget = getPos().x - target->getPos().x;
		if (playerPosFromTarget <= 0) {
			if (faceLeft)
				Turn();
		}
		else {
			if (!faceLeft)
				Turn();
		}
	}
	else {
		SoundManager::GetInstance()->playSound(SFX, "Click");
		TextBox::GetInstance()->setText("tooFar");
		TextBox::GetInstance()->SetDisplay(true);
		StopWalking();
	}
}

void Player::SetNextPosition(float x, float y)
{
	if (walk == false)
		SoundManager::GetInstance()->playSound(SFX, "Walking", true);
	float newX = x;
	if (x < walkLimit->getMinBound().x)
		newX = walkLimit->getMinBound().x;
	else if (x > walkLimit->getMaxBound().x)
		newX = walkLimit->getMaxBound().x;
	//std::cout << x << " " << newX << std::endl;
	next_position = glm::vec3(newX, y, 1);
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