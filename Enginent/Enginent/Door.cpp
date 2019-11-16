#include "Door.h"
#include "Game.h"


Door::Door(float px, float py, float cx, float cy){
	nextPlayerPos = glm::vec3(px, py, 1);
	nextCameraPos = glm::vec3(cx, cy, 1);
}

void Door::travel() {
	Game::GetInstance()->getPlayer()->SetPosition(nextPlayerPos);
	// set camera pos here
}