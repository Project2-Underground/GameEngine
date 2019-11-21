#include "Door.h"
#include "Game.h"
#include "Camera.h"

Door::Door(float px, float py, float cx, float cy){
	nextPlayerPos = glm::vec3(px, py, 1);
	nextCameraPos = glm::vec3(cx, cy, 1);
}

void Door::travel() {
	Player* player = Game::GetInstance()->getPlayer();
	Camera* camera = Camera::GetInstance();

	player->SetPosition(nextPlayerPos);
	player->setTarget(nextPlayerPos.x, nextPlayerPos.y);

	camera->SetPosition(nextCameraPos);
	camera->SetLimit(next_cam_limit);
}

void Door::SetNextCamLimit(Collider* lim) {
	next_cam_limit = lim;
}

void Door::action(int x, int y) {
	this->InteractableObj::action(x, y);
	// travel only when player stops walking
	travel();
}

Door::~Door() {
	if (next_cam_limit)
		delete next_cam_limit;
}