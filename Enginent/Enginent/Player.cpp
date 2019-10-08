#include "Player.h"
#include <iostream>

Player::Player()
{
	target = glm::vec3(this->pos);
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