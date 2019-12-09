#include "Door.h"
#include "Game.h"
#include "Camera.h"

Door::Door(float next_playerx, float next_playery, Collider* lim, std::string item_to_unlock) {
	interactType = DOOR;
	open = true;
	nextPlayerPos = glm::vec3(next_playerx, next_playery, 1);
	next_cam_limit = lim;
	if (item_to_unlock != "") {
		open = false;
		this->item_to_unlock = item_to_unlock;
	}
}

Door::Door(int next_room, int door_no) {
	this->next_room = next_room;
	this->door_no = door_no;
}

void Door::SetKey(std::string item_to_unlock) {
	open = false;
	this->item_to_unlock = item_to_unlock;
}

void Door::action() {
	Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
	if (open) {
		// new settings
		// load next room and next door position;


		// old settings
		Camera* camera = Camera::GetInstance();

		player->SetPosition(nextPlayerPos);
		player->SetNextPosition(nextPlayerPos.x, nextPlayerPos.y);

		//camera->SetPosition(nextCameraPos);
		if (next_cam_limit)
			camera->SetLimit(next_cam_limit);
		player->walk = false;
		camera->SetLimit(new Collider(glm::vec3(639, -1112, 1), glm::vec3(2817, 672, 1)));
		player->SetWalkLimit(new Collider(glm::vec3(639, -1112, 1), glm::vec3(2817, 672, 1)));
	}
	else {
		if (player->inventory->GetInventoryBox(0)->GetItem() != nullptr)
			Unlock(player->inventory->GetInventoryBox(0)->GetItem());
		if (!open) {
			SoundManager::GetInstance()->playSFX("Locked");
			this->InteractableObj::action();
		}
	}
}

void Door::SetDoorLevelPosition(glm::vec3 pos) {
	doorLevelPosition = pos;
}

Door::~Door() {
	if (next_cam_limit)
		delete next_cam_limit;
}

void Door::Unlock(InteractableObj* item) {
	if (item->object_name == item_to_unlock) {
		open = true;
		((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer()->inventory->removeItem((Item*)item);
	}
}