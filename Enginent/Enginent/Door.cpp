#include "InteractObj.h"
#include "Game.h"
#include "Camera.h"
#include "MouseInput.h"

Door::Door(std::string next_room, std::string next_door) {
	interactType = DOOR;
	this->nextRoom = next_room;
	this->nextDoor = next_door;
	used = true;
}

void Door::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	if (used) {
		// load next room and next door position;
		gs->ChangeRoom(nextRoom, nextDoor);
	}
	else if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		UseItem(gs->GetInventory()->GetSelectedItem());
	}
	else {
		SoundManager::GetInstance()->playSFX("Locked");
		this->InteractableObj::action();
	}
}