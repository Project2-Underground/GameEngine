#include "Player.h"
#include <iostream>

Player::Player()
{
	target = glm::vec3(this->pos);
	walk = false;
	dialogueText = new TextObject();
	dialogueColor.r = 255;
	dialogueColor.b = 255;
	dialogueColor.g = 255;
	dialogueColor.a = 0;
}

void Player::Move()
{
	if (this->pos.x > target.x)
	{
		if (pos.x - target.x > WALK_SPEED)
		{
			this->Translate(glm::vec3(-WALK_SPEED, 0, 0));
		}
		else
		{
			walk = false;
		}
	}
	else if (this->pos.x < target.x)
	{
		if (target.x - pos.x > WALK_SPEED)
		{
			this->Translate(glm::vec3(WALK_SPEED, 0, 0));
		}
		else
		{
			walk = false;
		}
	}
	col->Update();
}

void Player::setTarget(int x, int y)
{
	target = glm::vec3(x, y, 0);
	walk = true;
}

void Player::Walk(int x, int y)
{
	if (this->pos.x > target.x)
	{
		if (pos.x - target.x > WALK_SPEED)
		{
			this->Translate(glm::vec3(-WALK_SPEED, 0, 0));
		}
	}
	else if (this->pos.x < target.x)
	{
		if (target.x - pos.x > WALK_SPEED)
		{
			this->Translate(glm::vec3(WALK_SPEED, 0, 0));
		}
	}
	col->Update();
}


void Player::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
}

TextObject* Player::setDialogue(string dialogue)
{
	dialogueText->loadText(dialogue, dialogueColor, 18);
	dialogueText->SetPosition(glm::vec3(0.0f, -220.0f, 1.0f));
	return dialogueText;
}