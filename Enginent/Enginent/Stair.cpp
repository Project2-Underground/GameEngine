#include "Stair.h"
#include "Game.h"

Stair::Stair(float upperPosX, float upperPosY, float lowerPosX, float lowerPosY) {
	upperPos = glm::vec3(upperPosX, upperPosY, 1);
	lowerPos = glm::vec3(lowerPosX, lowerPosY, 1);
}

void Stair::action() {
	Player* player = Game::GetInstance()->getPlayer();
	if (player->getPos().y >= upperPos.y) {
		player->setTarget(lowerPos.x, lowerPos.y + (-1 * player->getSize().y / 2));
	}
	else if (player->getPos().y <= lowerPos.y) {
		player->setTarget(upperPos.x, upperPos.y + (-1 * player->getSize().y / 2));
	}
}