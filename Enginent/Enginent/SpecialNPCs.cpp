#include "SpecialNPCs.h"
#include "Game.h"
#include "InfoPhone.h"

Butler::Butler() { 
	currentPhase = PHASE0;
	object_name = "butler";
	interactType = TALK;
	turnLeft = true;
	Init(180, -409, glm::vec3(-987, -102, 1));
	col->setNewSize(350, -350);
	InitAnimator();
	anim->AddAnimation("Idle", "Texture/Character/NPCs/MainHallRoom/Butler.png", 2, 0.5f, true);
}
void Butler::Appear() {
	switch (currentPhase)
	{
	case Butler::PHASE0:
		if (!turnLeft) {
			SetScale(-1, 1);
			turnLeft = !turnLeft;
		}
		// after Elias got out of the room
		dialogue_name = "Butler_start";
		currentLevel = 1;
		MoveIn("MainHallUpper");
		SetPosition(glm::vec3(-987, -102, 1));
		disappearAfterAction = true;
		clickToInteract = false;
		triggered = false;
		break;
	case Butler::PHASE1:
		if (!turnLeft) {
			SetScale(-1, 1);
			turnLeft = !turnLeft;
		}
		currentLevel = 1;
		// after finished puzzle 3
		talk = false;
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
		if (!turnLeft) {
			SetScale(-1, 1);
			turnLeft = !turnLeft;
		}
		currentLevel = 1;
		talk = false;
		dialogue_name = "Hall_Bookshelf_R3";
		dialogue_after = "Butler1_end";
		MoveOut("MainHallUpper");
		MoveIn("MainHallLower");
		SetPosition(glm::vec3(1400, -90, 1));
		disappearAfterAction = false;
		clickToInteract = true;
		triggered = false;
		break;
	case Butler::PHASE3:
		if (!turnLeft) {
			SetScale(-1, 1);
			turnLeft = !turnLeft;
		}
		currentLevel = 2;
		break;
	case Butler::ROUTE_A:
		if (turnLeft) {
			SetScale(-1, 1);
			turnLeft = !turnLeft;
		}
		currentLevel = 3;
		dialogue_name = "Floor3A_butler";
		MoveIn("MainFloor3");
		disappearAfterAction = false;
		clickToInteract = true;
		triggered = false;
		SetPosition(glm::vec3(1974, -37, 1));
		break;
	case Butler::ROUTE_B: {
		if (turnLeft) {
			SetScale(-1, 1);
			turnLeft = !turnLeft;
		}
		currentLevel = 3;
		dialogue_name = "Floor3B_butler";
		MoveIn("MainFloor3");
		SetPosition(glm::vec3(-2185, -37, 1));
		disappearAfterAction = false;
		clickToInteract = false;
		triggered = false;
		// change others dialogues in floor3
		GameScreen* gs = (GameScreen*)Game::GetInstance()->GetScreen();
		Level* lvl = gs->GetCurrentLevel();
		lvl->rooms["MainFloor3"]->dialogue = "Floor3B";
		((InteractableObj*)lvl->FindObject("people1"))->SetCurrentDialogueName("Floor3B_people1");
		((InteractableObj*)lvl->FindObject("people2"))->SetCurrentDialogueName("Floor3B_people2");
		((InteractableObj*)lvl->FindObject("MainFloor3_House_DoorTrigger"))->SetCurrentDialogueName("Floor3B_factory");
	}break;
	}
	SetDisplay(true);
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
	case Butler::ROUTE_A:
		break;
	case Butler::ROUTE_B:
		break;
	default:
		break;
	}

	InteractableObj::action();
}
void Butler::Update() {
	anim->Update();
	if (triggered && display && disappearAfterAction && !TextBox::GetInstance()->IsDisplay()) {
		SetDisplay(false);
		col->enable = false;
		triggered = false;
	}
	if (!clickToInteract) {
		if (!triggered && col->isCollide(Game::GetInstance()->GetPlayer()->col)) {
			action();
			triggered = true;
			Game::GetInstance()->GetPlayer()->StopWalking();
		}
	}
	if (currentPhase == PHASE2 && !((GameScreen*)Game::GetInstance()->GetScreen())->IsPuzzleOpen() && !triggered) {
		Player* player = Game::GetInstance()->GetPlayer();
		TextBox::GetInstance()->setText(dialogue_name);
		dialogue_name = "Butler1_end";
		triggered = true;
	}
	else if (currentPhase == PHASE0 && !display && !triggered) {
		Phone::GetInstance()->Open();
		triggered = true;
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