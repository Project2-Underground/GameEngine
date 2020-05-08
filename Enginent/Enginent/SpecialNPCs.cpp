#include "SpecialNPCs.h"
#include "Game.h"
#include "InfoPhone.h"

Butler::Butler() { 
	currentPhase = PHASE0;
	object_name = "butler";
	interactType = TALK;
	triggered = true;
	Init(180, -409, glm::vec3(-987, -102, 1));
	col->setNewSize(350, -350);
	InitAnimator();
	anim->AddAnimation("Idle", "Texture/Character/NPCs/MainHallRoom/Butler.png", 2, 0.5f, true);
}
void Butler::Appear() {
	switch (currentPhase)
	{
	case Butler::PHASE0:
		// after Elias got out of the room
		dialogue_name = "Butler_start";
		currentLevel = 1;
		MoveIn("MainHallUpper");
		SetPosition(glm::vec3(-987, -102, 1));
		disappearAfterAction = true;
		clickToInteract = false;
		break;
	case Butler::PHASE1:
		currentLevel = 1;
		// after finished puzzle 3
		dialogue_name = "Butler1";
		dialogue_after = "Butler1_after";
		MoveOut("MainHallUpper");
		MoveIn("MainHallLower");
		SetPosition(glm::vec3(1400, -90, 1));
		disappearAfterAction = false;
		clickToInteract = true;
		//ChangeDialogue();
		break;
	case Butler::PHASE2:
		currentLevel = 1;
		dialogue_name = "Butler1_end";
		dialogue_after = "Butler1_end";
		MoveOut("MainHallUpper");
		MoveIn("MainHallLower");
		disappearAfterAction = false;
		clickToInteract = true;
		break;
	case Butler::PHASE3:
		currentLevel = 2;
		break;
	default:
		break;
	}
	SetDisplay(true);
	triggered = false;
	col->enable = true;
}
void Butler::action() {
	switch (currentPhase)
	{
	case Butler::PHASE0:
		((GameScreen*)Game::GetInstance()->GetScreen())->phoneIcon->Appear();
		Phone::GetInstance()->Message("Unknown1", 7);
		break;
	case Butler::PHASE1:
		break;
	case Butler::PHASE2:
		break;
	case Butler::PHASE3:
		break;
	default:
		break;
	}

	InteractableObj::action();
}
void Butler::Update() {
	if (anim)
		anim->Update();
	if (triggered && display && disappearAfterAction && !TextBox::GetInstance()->IsDisplay()) {
		SetDisplay(false);
		col->enable = false;
	}
	if (!clickToInteract) {
		PlayerTriggerObj::Update();
	}
}
void Butler::SetTriggered(bool b) {
	triggered = b;
}
bool Butler::IsTriggered() {
	return triggered;
}
void BackAlleyEmma::action() {
	NonPlayer::action();
	GameScreen* gs = (GameScreen*)Game::GetInstance()->GetScreen();
	RemoveObj* obj = (RemoveObj*)gs->GetCurrentLevel()->FindObject("Building1_FloorDoorClose");
	obj->Trigger();
}
void Butler::MoveOut(std::string roomName) {
	Level* lvl = Game::GetInstance()->GetCurrentLevel();
	if (currentLevel == lvl->levelNo) {
		std::vector<DrawableObject*>::iterator itr;
		itr = std::find(lvl->rooms[roomName]->npcs.begin(), lvl->rooms[roomName]->npcs.end(), this);
		if (itr != lvl->rooms[roomName]->npcs.end())
			lvl->rooms[roomName]->npcs.erase(itr);
	}
}
void Butler::MoveIn(std::string roomName) {
	Level* lvl = Game::GetInstance()->GetCurrentLevel();
	if (currentLevel == lvl->levelNo) {
		std::vector<DrawableObject*>::iterator itr;
		itr = std::find(lvl->rooms[roomName]->npcs.begin(), lvl->rooms[roomName]->npcs.end(), this);
		if (itr == lvl->rooms[roomName]->npcs.end())
			lvl->rooms[roomName]->npcs.push_back(this);
	}
}