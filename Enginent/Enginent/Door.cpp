#include "Door.h"
#include "Game.h"
#include "Camera.h"
#include "MouseInput.h"

Door::Door(std::string next_room, std::string next_door) {
	interactType = DOOR;
	this->nextRoom = next_room;
	this->nextDoor = next_door;
	triggered = false;
	used = true;
	isOpenDoor = false;
	hasNextX = false;
	openTexture = 0;
}

void Door::ChangePortal(std::string next_room, std::string next_door) {
	this->nextRoom = next_room;
	this->nextDoor = next_door;
}

void Door::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	if (used || triggered) {
		if (interactType != STAIR && !isOpenDoor) {
			SoundManager::GetInstance()->playSound(SFX, "OpenDoor", false);
			Game::GetInstance()->GetPlayer()->anim->Play("Pickup", false);
		}
		if (hasOpenTexture && openTexture != texture) {
			SetTexture(openTexture);
			hasOpenTexture = false;
		}
		else
			gs->ChangeRoom(nextRoom, nextDoor);
	}
	else if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		UseItem(gs->GetInventory()->GetSelectedItem());
	}
	else{
		Game::GetInstance()->GetPlayer()->anim->Play("Pickup", false);
		SoundManager::GetInstance()->playSound(SFX, "Locked");
		InteractableObj::action();
	}
}

void Door::SetOpenTexture(std::string texture) {
	openTexture = Game::GetInstance()->GetRenderer()->LoadTexture(texture);
	hasOpenTexture = true;
}

void Door::SetPlayerNextX(float x) {
	playerNextX = x;
	hasNextX = true;
}

void Door::Trigger() {
	triggered = true;
	interactType = DOOR;
}

void Door::Open() {
	used = true; 
	interactType = DOOR;
	if(hasOpenTexture)
		SetTexture(openTexture);
}

void WallDoor::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	//std::cout << "WallDoor::action() " << used << " " << triggered << std::endl;
	if (used || triggered) {
		interactType = DOOR;
		if (!dialogue_after_trigger.empty()) {
			TextBox::GetInstance()->setText(dialogue_after_trigger);
			dialogue_after_trigger.clear();
		}else
			Door::action();
	}
	else if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		UseItem(gs->GetInventory()->GetSelectedItem());
	}
	else {
		InteractableObj::action();
		interactType = NORMAL;
	}
}
void SecretDoor::Open() {
	used = true;
	interactType = DOOR;
}
void SecretDoor::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	if (used || triggered) {
		if (GetTexture() != openTexture && openTexture != 0) {
			SetTexture(openTexture);
			TextBox::GetInstance()->setText("Building1_FloorDoorClose");
		}
		else
			gs->ChangeRoom(nextRoom, nextDoor);
	}
	else if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		UseItem(gs->GetInventory()->GetSelectedItem());
		if (used)
			interactType = DOOR;
	}
	else {
		InteractableObj::action();
	}
}
void ChangeLevelDoor::action() {
	Game::GetInstance()->TriggerChangeLevel(nextLevel);
}