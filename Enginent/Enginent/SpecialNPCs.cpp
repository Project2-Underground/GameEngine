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
	anim->AddAnimation("Idle", "Texture/Character/NPCs/MainHallRoom/Butler.png", 2, 0.25f, true);
}
void Butler::Appear() {
	switch (currentPhase)
	{
	case Butler::PHASE0:
		Game::GetInstance()->GetCurrentLevel()->rooms["MainHallUpper"]->npcs.push_back(this);
		SetPosition(glm::vec3(-987, -102, 1));
		disappearAfterAction = true;
		clickToInteract = false;
		break;
	case Butler::PHASE1:
		Game::GetInstance()->GetCurrentLevel()->rooms["MainHallUpper"]->npcs.erase(std::find(Game::GetInstance()->GetCurrentLevel()->rooms["MainHallUpper"]->npcs.begin(), Game::GetInstance()->GetCurrentLevel()->rooms["MainHallUpper"]->npcs.end(), this));
		/*Game::GetInstance()->GetCurrentLevel()->rooms["MainHallUpper"]->npcs.push_back(this);
		SetPosition(glm::vec3(-987, -102, 1));*/
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
	Game::GetInstance()->GetPlayer()->StopWalking();
}
void Butler::SetTriggered(bool b) {
	triggered = b;
}
bool Butler::IsTriggered() {
	return triggered;
}