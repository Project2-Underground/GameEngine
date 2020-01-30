#include "InteractObj.h"
#include "Game.h"
#include "Camera.h"

Door::Door(std::string next_room, std::string next_door) {
	interactType = DOOR;
	this->nextRoom = next_room;
	this->nextDoor = next_door;
	used = true;
}

void Door::action() {
	Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();
	if (used) {
		// load next room and next door position;
		((GameScreen*)Game::GetInstance()->GetScreen())->ChangeRoom(nextRoom, nextDoor);
	}
	else {
		SoundManager::GetInstance()->playSFX("Locked");
		this->InteractableObj::action();
	}
}

void Door::UseItem(Item* item) {
	if (item->name == item_to_use) {
		used = true;
		((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory()->removeItem((Item*)item);
	}
}