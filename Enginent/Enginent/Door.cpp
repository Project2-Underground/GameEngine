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

void Door::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	if (used || triggered) {
		if (interactType != STAIR && !isOpenDoor) {
			SoundManager::GetInstance()->playSound(SFX, "OpenDoor", false);
			Game::GetInstance()->GetPlayer()->anim->Play("Pickup", false);
		}
		if (GetTexture() != openTexture && openTexture != 0)
			SetTexture(openTexture);
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
	SetTexture(openTexture);
}

void WallDoor::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	std::cout << "WallDoor::action() " << used << " " << triggered << std::endl;
	if (triggered) {
		interactType = DOOR;
		TextBox::GetInstance()->setText(dialogue_after_trigger);
		dialogue_after_trigger.clear();
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
void SecretDoor::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	if (used || triggered) {
		if (GetTexture() != openTexture && openTexture != 0)
			SetTexture(openTexture);
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
void EliasDoor::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	if (used) {
		Game::GetInstance()->GetPlayer()->anim->Play("Pickup", false);
		SoundManager::GetInstance()->playSound(SFX, "OpenDoor", false);
		gs->ChangeRoom(nextRoom, nextDoor);
	}
	else if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		UseItem(gs->GetInventory()->GetSelectedItem());
		TextBox::GetInstance()->setText("EliasRoom_door_useKey");
	}
	else {
		Door::action();
	}
}
void ChangeLevelDoor::action() {
	Game::GetInstance()->TriggerChangeLevel(nextLevel);
}