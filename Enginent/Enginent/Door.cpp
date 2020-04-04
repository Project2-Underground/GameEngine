#include "Door.h"
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
		gs->ChangeRoom(nextRoom, nextDoor);
	}
	else if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		UseItem(gs->GetInventory()->GetSelectedItem());
	}
	else{
		SoundManager::GetInstance()->playSound(SFX, "Locked");
		InteractableObj::action();
	}
}

void Door::SetOpenTexture(std::string texture) {
	used = false;
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
	}
}

void EliasDoor::Init() {
	anim = new Animator();
	// add animation
	anim->AddAnimation("Idle1", GetTexture(), 1, 0, true);
	// Idle2
	anim->AddAnimation("Idle2", "", 1, 0, true);
	// Transform
	anim->AddAnimation("Transform", "", 1, 0);
}

void EliasDoor::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	if (used) {
		// call text dialogue when open the door
		anim->SetDefaultAnimation("Idle2");
		triggerAnim = true;
	}
	else if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		// call text dialogue when use the key
		UseItem(gs->GetInventory()->GetSelectedItem());
	}
	else {
		SoundManager::GetInstance()->playSound(SFX, "Locked");
		InteractableObj::action();
	}
}

void EliasDoor::Update() {
	if (triggerAnim) {
		anim->Play("Tranform");
		triggerAnim = false;
	}else if(!anim->IsPlaying("Transform"))
		((GameScreen*)Game::GetInstance()->GetScreen())->ChangeRoom(nextRoom, nextDoor);

}