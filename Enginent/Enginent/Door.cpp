#include "Door.h"
#include "Game.h"
#include "Camera.h"

Door::Door(float next_playerx, float next_playery, Collider* lim, InteractableObj* item_to_unlock) {
	open = true;
	nextPlayerPos = glm::vec3(next_playerx, next_playery, 1);
	next_cam_limit = lim;
	if (item_to_unlock) {
		open = false;
		this->item_to_unlock = item_to_unlock;
	}
}

void Door::action() {
	Unlock(Game::GetInstance()->getPlayer()->inventory->GetInventoryBox(0)->GetItem());
	if (open) {
		Player* player = Game::GetInstance()->getPlayer();
		Camera* camera = Camera::GetInstance();

		player->SetPosition(nextPlayerPos);
		player->SetNextPosition(nextPlayerPos.x, nextPlayerPos.y);

		//camera->SetPosition(nextCameraPos);
		if (next_cam_limit)
			camera->SetLimit(next_cam_limit);
		player->walk = false;
	}
	else {
		this->InteractableObj::action();
	}
}

Door::~Door() {
	if (next_cam_limit)
		delete next_cam_limit;
}

void Door::Unlock(InteractableObj* item) {
	if (item == item_to_unlock) {
		open = true;
		Game::GetInstance()->getPlayer()->inventory->removeItem((Item*)item);
	}
}