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
	Level* lvl = Game::GetInstance()->GetCurrentLevel();
	std::vector<DrawableObject*>::iterator itr;
	switch (currentPhase)
	{
	case Butler::PHASE0:
		// after Elias got out of the room
		lvl->rooms["MainHallUpper"]->npcs.push_back(this);
		SetPosition(glm::vec3(-987, -102, 1));
		disappearAfterAction = true;
		clickToInteract = false;
		break;
	case Butler::PHASE1:
		// after finished puzzle 3
		itr = std::find(lvl->rooms["MainHallUpper"]->npcs.begin(), lvl->rooms["MainHallUpper"]->npcs.end(), this);
		if(itr != lvl->rooms["MainHallUpper"]->npcs.end())
			lvl->rooms["MainHallUpper"]->npcs.erase(itr);
		lvl->rooms["MainHallLower"]->npcs.push_back(this);
		SetPosition(glm::vec3(1400, -90, 1));
		disappearAfterAction = false;
		clickToInteract = true;
		//ChangeDialogue();
		break;
	default:
		break;
	}
	SetDisplay(true);
	triggered = false;
	col->enable = true;
}
void Butler::Update() {
	if(anim)
		anim->Update();
	if (triggered && display && disappearAfterAction) {
		SetDisplay(false);
		col->enable = false;
	}
	if (!clickToInteract) {
		PlayerTriggerObj::Update();
	}
}
void Butler::action() {
	switch (currentPhase)
	{
	case Butler::PHASE0:
		dialogue_name = "Butler_start";
		((GameScreen*)Game::GetInstance()->GetScreen())->phoneIcon->Appear();
		Phone::GetInstance()->Message("Unknown", 7);
		break;
	case Butler::PHASE1:
		break;
	default:
		break;
	}

	InteractableObj::action();
}
void Butler::SetTriggered(bool b) {
	triggered = b;
}
bool Butler::IsTriggered() {
	return triggered;
}