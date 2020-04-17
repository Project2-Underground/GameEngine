#include "InteractObj.h"
#include "Game.h"
#include "GameWindows.h"
#include "InfoPhone.h"
#include "GameWindows.h"

InteractableObj::InteractableObj() {
	interactType = NORMAL;
	triggered = true;
}

void InteractableObj::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
	/*std::cout << "collider min bound: " << col->getMinBound().x << ", " << col->getMinBound().y << std::endl;
	std::cout << "collider max bound: " << col->getMaxBound().x << ", " << col->getMaxBound().y << std::endl;*/
}

void InteractableObj::TakePic() {
	if (takePic) {
		Phone* phone = Phone::GetInstance();
		phone->AddPage(NOTE, picName);
		takePic = false;
	}
}

Item* InteractableObj::GetItem() {
	return ((GameScreen*)Game::GetInstance())->FindItem(itemName);
}
void InteractableObj::SetDialogueName(std::string n)
{
	this->dialogue_name = n;
}

void InteractableObj::action() {
	if (dialogue_name != "")
	{
		TextBox::GetInstance()->setText(this->dialogue_name);
		TextBox::GetInstance()->SetDisplay(true);
	}

	//if (actionTriggerDialogue) {
	//	actionTriggerDialogue = false;
	//	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	//	for (auto obj : dialogueTriggers[ACTION_TRIGGER_DIALOGUE])
	//		gs->GetCurrentLevel()->TriggerChangeDialogue(obj.first, obj.second);
	//}
	Game::GetInstance()->GetPlayer()->anim->Play("Idle");
	for (auto obj : triggerObjs)
		obj->triggered = true;
	PickUpItem();
	TakePic();
}

void InteractableObj::SetTakePic(std::string pic) {
	picName = pic;
	takePic = true;
}

bool InteractableObj::CheckCollider(float x, float y) {
	return this->col->isClicked(x, y);
}
bool InteractableObj::CheckPointing(float x, float y)
{
	if (this->col->isClicked(x, y))
		return true;
	return false;
}

InteractableObj::~InteractableObj() {
	if (col)
		delete col;
}

bool InteractableObj::operator==(const InteractableObj& obj) {
	if (this->object_name == obj.object_name) {
		return true;
	}
	return false;
}

void InteractableObj::SetItemToUse(std::string item_to_unlock) {
	this->item_to_use = item_to_unlock;
	used = false;
}

void InteractableObj::UseItem(Item* item) {
	if (item != nullptr && item_to_use == item->name) {
		//std::cout << "item is used and removed from the inventory\n";
		used = true;
		((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory()->RemoveItem(item);
	}
}

void InteractableObj::PickUpItem() {
	if (hasItem) {
		GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen()); 
		Item* item = gs->FindItem(itemName);
		gs->GetInventory()->AddItem(item);
		gs->GetPlayer()->anim->Play("Pickup", false);

		ViewWindow* vw = ViewWindow::GetInstance();
		vw->Open();
		vw->SetViewItem(item->GetViewTexture());

		hasItem = false;
		interactType = NORMAL;
		/*if (useItemTriggerDialogue) {
			useItemTriggerDialogue = false;
			actionTriggerDialogue = false;
			for (auto obj : dialogueTriggers[USE_ITEM_TRIGGER_DIALOGUE])
				gs->GetCurrentLevel()->TriggerChangeDialogue(obj.first, obj.second);
		}*/
	}	
}

void InteractableObj::SetAnimation(std::string name, std::string texture, int frameNo, float frameRate, bool loop) {
	if (anim == nullptr)
		anim = new Animator();

	anim->AddAnimation(name, texture, frameNo, frameRate, loop);
}

void InteractableObj::AddTriggerObj(InteractableObj* obj) {
	triggerObjs.push_back(obj);
}

//
//void InteractableObj::AddTriggerDialogue(DialogueTrigger type, std::string objName, std::string dName) {
//	dialogueTriggers[type].insert(std::pair<std::string, std::string>(objName, dName));
//	switch (type)
//	{
//		case ACTION_TRIGGER_DIALOGUE: actionTriggerDialogue = true; break;
//		case USE_ITEM_TRIGGER_DIALOGUE: useItemTriggerDialogue = true; break;
//	}
//}

void InteractableObj::ChangeDialogue(std::string n)
{
	this->dialogue_name = n;
}

OpenObj::OpenObj() {
	interactType = PICKUP;
	open = false;
	//item = nullptr;
}

void OpenObj::SetOpenTexture(std::string openT) {
	openTexture = Game::GetInstance()->GetRenderer()->LoadTexture(openT);
	nextTexture = Game::GetInstance()->GetRenderer()->LoadTexture(openT);
}

void OpenObj::SetNextTexture(std::string next) {
	nextTexture = Game::GetInstance()->GetRenderer()->LoadTexture(next);
}

void OpenObj::action() {
	if (!open && triggered) {
		Game::GetInstance()->GetPlayer()->anim->Play("Idle");
		Open();
	}
	else if (open) {
		PickUpItem();
		SetTexture(nextTexture);
	}
	else {
		if (dialogue_name != "")
		{
			TextBox::GetInstance()->setText(this->dialogue_name);
			TextBox::GetInstance()->SetDisplay(true);
		}
	}
}

void OpenObj::Open() {
	open = true;
	//std::cout << "open\n";
	if (dialogue_name != "")
	{
		TextBox::GetInstance()->setText(this->dialogue_name);
		TextBox::GetInstance()->SetDisplay(true);
	}
	SetTexture(openTexture);
	if (hasItem)interactType = PICKUP;
	else interactType = NORMAL;
}

void OpenObj::ClearItem() {
	interactType = NORMAL;
	SetTexture(nextTexture);
}

OpenObj::~OpenObj() {
}

void ViewObj::SetViewTexture(std::string view) {
	viewTexture = Game::GetInstance()->GetRenderer()->LoadTexture(view);
}

void ViewObj::action() {
	ViewWindow* vw = ViewWindow::GetInstance();
	vw->Open();
	vw->SetViewItem(viewTexture);
	InteractableObj::action();
	/*TextBox::GetInstance()->setText(this->dialogue_name);
	TakePic();*/
	// set description
}

void NonPlayer::action()
{
	if (dialogue_name != "")
	{
		TextBox::GetInstance()->setText(this->dialogue_name);
		TextBox::GetInstance()->SetDisplay(true);
	}
	Game::GetInstance()->GetPlayer()->anim->Play("Idle");
	for (auto obj : triggerObjs)
		obj->triggered = true;
	TakePic();
}

void PuzzleObj::SetPuzzleName(std::string name) {
	puzzleName = name;
}

void PuzzleObj::action() {
	//std::cout << "click puzzle\n";
	if (((GameScreen*)Game::GetInstance()->GetScreen())->puzzles[puzzleName]->CheckRequirements())
		((GameScreen*)Game::GetInstance()->GetScreen())->OpenPuzzle(puzzleName);
	else
		InteractableObj::action();
}

void SaveObj::action() {
	Game::GetInstance()->SetSaveGame(true);
	SaveLoadWindow::GetInstance()->Open();
}
void PlayerTriggerObj::Update() {
	if (!triggered && col->isCollide(Game::GetInstance()->GetPlayer()->col)) {
		action();
		triggered = true;
	}
}

Butler::Butler() { 
	object_name = "butler";
	interactType = TALK; 
	triggered = false; 
	Init(100, 100, glm::vec3(-200, 0, 0));
	InitAnimator();
	anim->AddAnimation("Idle", "", 2, 0.25f, true);
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
void Butler::SetTriggered(bool b) {
	triggered = b;
}
bool Butler::IsTriggered() {
	return triggered;
}