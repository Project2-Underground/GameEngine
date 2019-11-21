#include "Player.h"
#include "SquareMeshVbo.h"
#include "Game.h"
#include <iostream>

Player::Player()
{
	target = glm::vec3(this->pos);
	anim = new Animator();

	Animation* move = new Animation("Move", "Texture/Character/Elias_walk.png");
	Animation* idle = new Animation("Idle", "Texture/Character/Elias_idle.png");
	move->SetFrame(4);
	move->SetFramePeriod(0.16f);

	idle->SetFrame(1);
	idle->SetFramePeriod(0.0f);

	anim->AddAnimation(idle);
	anim->AddAnimation(move);
	anim->SetDefaultAnimation("Move");

	walkSound = SoundManager::GetInstance()->play2D("Sound/walking_sound.mp3", true, true, true);

	walk = false;
	faceLeft = true;
	dialogueText = new TextObject();
	dialogueColor.r = 255;
	dialogueColor.b = 255;
	dialogueColor.g = 255;
	dialogueColor.a = 0;
	dialogue = " ";

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
	Move();
	if (!walk)
	{
		anim->Play("Idle", true);
		std::cout << "stop sound";
		walkSound->setIsPaused(true);
		walkSound->setPlayPosition(0);
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
	if (this->display)
	{
		target = realPos;
	}
	walkSound->setIsPaused(false);
	walk = true;
	setDialogue(" ");
	if(!(anim->currentAnimation->animationName == "Move"))
		anim->Play("Move", true);
}

void Player::setTarget(float x, float y)
{
	target = glm::vec3(x, y , 1);
	walk = true;
	setDialogue(" ");
	if (!(anim->currentAnimation->animationName == "Move"))
		anim->Play("Move", true);

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

Player::~Player() {
	if (col)
		delete col;
	if (inventory)
		delete inventory;
	if (dialogueText)
		delete dialogueText;
}