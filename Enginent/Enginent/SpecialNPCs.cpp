#include "SpecialNPCs.h"
#include "Game.h"
#include "InfoPhone.h"

Butler::Butler() { 
	currentPhase = PHASE0;
	object_name = "butler";
	interactType = TALK;
	triggered = false;
	Init(180, -409, glm::vec3(-987, -102, 1));
	col->setNewSize(350, 350);
	InitAnimator();
	anim->AddAnimation("Idle", "Texture/Character/NPCs/MainHallRoom/Butler.png", 2, 0.25f, true);
}
void Butler::Appear(glm::vec3 pos, std::string dialogue) {
	SetPosition(pos);
	SetDisplay(true);
	triggered = false;
}
void Butler::Update() {
	if (triggered && display)
		SetDisplay(false);
	PlayerTriggerObj::Update();
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