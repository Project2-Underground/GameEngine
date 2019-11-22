#include "Door.h"
#include "Game.h"
#include "Camera.h"

Door::Door(float next_playerx, float next_playery, float next_camx, float next_camy){
	nextPlayerPos = glm::vec3(next_playerx, next_playery, 1);
	nextCameraPos = glm::vec3(next_camx, next_camy, 1);
	next_cam_limit = nullptr;
}

void Door::travel() {
	Player* player = Game::GetInstance()->getPlayer();
	Camera* camera = Camera::GetInstance();

	player->SetPosition(nextPlayerPos);
	player->setTarget(nextPlayerPos.x, nextPlayerPos.y);

	//camera->SetPosition(nextCameraPos);
	if (next_cam_limit)
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