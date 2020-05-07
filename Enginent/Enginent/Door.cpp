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

void Door::Open() {
	used = true;
	SetTexture(openTexture);
}

void WallDoor::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	if (used) {
		gs->ChangeRoom(nextRoom, nextDoor);
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
	if (used) {
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