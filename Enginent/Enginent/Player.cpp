#include "Player.h"
#include "SquareMeshVbo.h"
#include <iostream>

Player::Player()
{
	target = glm::vec3(this->pos);
	anim = new Animator();

	Animation* move = new Animation("Move", "Texture/move_test.png");
	Animation* idle = new Animation("Idle", "Texture/idle_test2.png");
	move->SetFrame(6);
	move->SetFramePeriod(0.06f); 

	idle->SetFrame(8);
	idle->SetFramePeriod(0.09f);

	anim->AddAnimation(idle);
	anim->AddAnimation(move);
	anim->SetDefaultAnimation("Idle");

}

void Player::Move()
{
	if (this->pos.x > target.x)
	{
		if (pos.x - target.x > 3)
		{
			this->Translate(glm::vec3(-3, 0, 0));
		}
	}
	else if (this->pos.x < target.x)
	{
		if (target.x - pos.x > 3)
		{
			this->Translate(glm::vec3(3, 0, 0));
		}
	}
	col->Update();

}

void Player::setTarget(int x, int y)
{
	target = glm::vec3(x, y, 0);
}