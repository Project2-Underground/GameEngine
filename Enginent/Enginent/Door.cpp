#include "Door.h"
#include "Game.h"
#include "Camera.h"

Door::Door(std::string next_room, std::string next_door) {
	this->nextRoom = next_room;
	this->nextDoor = next_door;
	open = true;
}

void Door::SetKey(std::string item_to_unlock) {
	this->item_to_unlock = item_to_unlock;
	open = false;
}

void Door::action() {
	Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
	if (open) {
		// load next room and next door position;
		((GameScreen*)Game::GetInstance()->GetScreen())->ChangeRoom(nextRoom, nextDoor);
	}
	else {
		SoundManager::GetInstance()->playSFX("Locked");
		this->InteractableObj::action();
	}
}

void Door::Unlock(InteractableObj* item) {
	if (item->object_name == item_to_unlock) {
		open = true;
		((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer()->inventory->removeItem((Item*)item);
	}
}