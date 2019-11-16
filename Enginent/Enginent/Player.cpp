#include "Player.h"
#include "SquareMeshVbo.h"
#include <iostream>

Player::Player()
{
	target = glm::vec3(this->pos);
	anim = new Animator();

	Animation* move = new Animation("Move", "Texture/Character/Elias_move.png");
	Animation* idle = new Animation("Idle", "Texture/idle_test2.png");
	move->SetFrame(8);
	move->SetFramePeriod(0.08f);

	idle->SetFrame(8);
	idle->SetFramePeriod(0.09f);

	anim->AddAnimation(idle);
	anim->AddAnimation(move);
	anim->SetDefaultAnimation("Move");


	walk = false;
	faceLeft = true;
	dialogueText = new TextObject();
	dialogueColor.r = 255;
	dialogueColor.b = 255;
	dialogueColor.g = 255;
	dialogueColor.a = 0;
	dialogue = " ";

	const int inventoryNum = 5;
	glm::vec3 inventoryBoxPos[inventoryNum] = { glm::vec3(0,0,1), glm::vec3(100,500,1), glm::vec3(500,100,1), glm::vec3(-500,100,1), glm::vec3(-600,-800,1) };
	inventory = new Inventory(inventoryNum, inventoryBoxPos, 50);
}

void Player::Update()
{
	Move();
	if (!walk)
	{
		dialogueText->loadText(dialogue, dialogueColor, 18);
		dialogueText->SetPosition(glm::vec3(0.0f, -220.0f, 1.0f));
	}
}

void Player::Move()
{
	if (this->pos.x > target.x)
	{
		if (pos.x - target.x > WALK_SPEED)
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
			walk = false;
		}
	}
	else if (this->pos.x < target.x)
	{
		if (target.x - pos.x > WALK_SPEED)
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
			walk = false;
		}
	}
	col->Update();
}

void Player::setTarget(glm::vec3 realPos)
{
	target = realPos;
	walk = true;
	setDialogue(" ");
}


void Player::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
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