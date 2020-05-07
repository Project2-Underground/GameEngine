#include "InteractObj.h"
#include "Game.h"
#include "GameWindows.h"
#include "InfoPhone.h"
#include "GameWindows.h"
#include "MouseInput.h"

InteractableObj::InteractableObj() {
	interactType = NORMAL;
	triggered = true;
	talk = false;
	used = true;
}

void InteractableObj::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
	/*std::cout << "collider min bound: " << col->getMinBound().x << ", " << col->getMinBound().y << std::endl;
	std::cout << "collider max bound: " << col->getMaxBound().x << ", " << col->getMaxBound().y << std::endl;*/
}

void InteractableObj::TakeNote() {
	//std::cout << (takeNote ? "yes" : "no") << "\n";
	if (takeNote) {
		interactType = NORMAL;
		col->enable = false;
		Phone* phone = Phone::GetInstance();
		phone->AddPage(NOTE, noteName);
		takeNote = false;
		SoundManager::GetInstance()->playSound(SFX, "CollectNote", false);
	}
}

Item* InteractableObj::GetItem() {
	return ((GameScreen*)Game::GetInstance())->FindItem(itemName);
}
void InteractableObj::SetDialogueName(std::string n, std::string a)
{
	this->dialogue_name = n;
	this->dialogue_before = n;
	this->dialogue_after = a;
}

void InteractableObj::action() {
	if (dialogue_name != "")
	{
		TextBox::GetInstance()->setText(this->dialogue_name, talk);
		if (!talk)
		{
			talk = true;
			if(dialogue_after != "")
				dialogue_name = dialogue_after;
		}
		TextBox::GetInstance()->SetDisplay(true);
	}
	if(interactType != DOOR)
		Game::GetInstance()->GetPlayer()->anim->Play("Idle");
	for (auto obj : triggerObjs)
		obj->triggered = true;
	PickUpItem();
	TakeNote();
}

void InteractableObj::SetNoteName(std::string note) {
	noteName = note;
	takeNote = true;
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
		Inventory* i = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
		used = true;
		if(!item->multipleUse)
			i->RemoveItem(item);

		i->UnselectItem();
	}
}

void InteractableObj::PickUpItem() {
	if (hasItem && !scriptHandleItem) {
		GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen()); 
		Item* item = gs->FindItem(itemName);
		gs->GetInventory()->AddItem(item);
		gs->GetPlayer()->anim->Play("Pickup", false);

		ViewWindow* vw = ViewWindow::GetInstance();
		vw->Open();
		vw->SetViewItem(item);

		hasItem = false;
		if(interactType == PICKUP)
			interactType = NORMAL;
		if(hasNextTexture)
			SetTexture(nextTexture);

		SoundManager::GetInstance()->playSound(SFX, "Pickup", false);
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

void InteractableObj::ChangeDialogue(std::string n, std::string a)
{
	this->dialogue_before = n;
	this->dialogue_after = a;
	if (talk)
	{
		this->dialogue_name = a;
	}
	else
	{
		this->dialogue_name = n;
	}
}

void InteractableObj::SetTalked(bool b) {
	talk = b;
	if (talk) {
		if (dialogue_after != "")
			dialogue_name = dialogue_after;
	}
}

void InteractableObj::SetNextTexture(std::string next) {
	nextTexture = Game::GetInstance()->GetRenderer()->LoadTexture(next);
	hasNextTexture = true;
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

void OpenObj::action() {
	if (!open && triggered) {
		Game::GetInstance()->GetPlayer()->anim->Play("Pickup", false);
		Open();
	}
	else if (open) {
		PickUpItem();
	}
	else {
		if (dialogue_name != "")
		{
			TextBox::GetInstance()->setText(this->dialogue_name, talk);
			if (!talk)
			{
				talk = true;
				if (dialogue_after != "")
					dialogue_name = dialogue_after;
			}
			TextBox::GetInstance()->SetDisplay(true);
		}
	}
}

void OpenObj::SetSound(std::string s) {
	sound = s;
}

void OpenObj::Open() {
	open = true;
	//std::cout << "open\n";
	if (dialogue_name != "")
	{
		TextBox::GetInstance()->setText(this->dialogue_name, talk);
		if (!talk)
		{
			talk = true;
			if (dialogue_after != "")
			dialogue_name = dialogue_after;
		}
		TextBox::GetInstance()->SetDisplay(true);
	}
	if (!sound.empty()) {
		SoundManager::GetInstance()->playSound(SFX, sound, false);
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

unsigned int ViewObj::GetViewTexture() {
	return viewTexture;
}

void ViewObj::action() {
	ViewWindow* vw = ViewWindow::GetInstance();
	vw->Open();
	vw->SetViewItem(this);
	InteractableObj::action();
}

void NonPlayer::action()
{
	Phone::GetInstance()->AddNPCInteracted(object_name);
	//std::cout << interactType << std::endl;
	if (dialogue_name != "")
	{
		TextBox::GetInstance()->setText(this->dialogue_name, talk);
		if (!talk)
		{
			talk = true;
			if (dialogue_after != "")
			dialogue_name = dialogue_after;
		}
		TextBox::GetInstance()->SetDisplay(true);
	}
	Game::GetInstance()->GetPlayer()->anim->Play("Idle");
	for (auto obj : triggerObjs)
		obj->triggered = true;
	TakeNote();
}

void PuzzleObj::SetPuzzleName(std::string name) {
	puzzleName = name;
}

void PuzzleObj::action() {
	if (used && ((GameScreen*)Game::GetInstance()->GetScreen())->puzzles[puzzleName]->CheckRequirements()) {
		if (dialogue_name != "") {
			TextBox::GetInstance()->setText(dialogue_name);
			// tmp solution
			dialogue_name = "";
		}
		else {
			Puzzle* p = ((GameScreen*)Game::GetInstance()->GetScreen())->puzzles[puzzleName];
			TextBox::GetInstance()->setText(p->prepTalk);
			((GameScreen*)Game::GetInstance()->GetScreen())->OpenPuzzle(puzzleName);
		}
	}
	else if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) 
		UseItem(((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory()->GetSelectedItem());
	else
		InteractableObj::action();
}

void SaveObj::action() {
	Game::GetInstance()->SetSaveGame(true);
	SaveLoadWindow::GetInstance()->Open();
}
PlayerTriggerObj::PlayerTriggerObj() {
	triggered = false;
}
void PlayerTriggerObj::Update() {
	if (!triggered && col->isCollide(Game::GetInstance()->GetPlayer()->col)) {
		action();
		triggered = true;

		Game::GetInstance()->GetPlayer()->StopWalking();
	}
}

NumpadPuzzleAfter::NumpadPuzzleAfter() {
	object_name = "NumpadAfterUnlock";
	dialogue_name = "backdoor_com_pass";
	dialogue_after = "backdoor_com_pass";
	actionDone = false;
	SetItemToUse("keyCard");
	//SetDialogueName();
}
void NumpadPuzzleAfter::action() {
	if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
		UseItem(gs->GetInventory()->GetSelectedItem());

		if (used && !actionDone) {
			TextBox::GetInstance()->setText("backdoor_card");
			UnlockBookshelf();
		}
		
	}
	else if (!used) {
		InteractableObj::action();
	}
}

void NumpadPuzzleAfter::UnlockBookshelf() {
	actionDone = true;
	Game* g = Game::GetInstance();
	PuzzleObj* tmp = new PuzzleObj();
	InteractableObj* puzzleObj = (InteractableObj*)g->GetCurrentLevel()->rooms["MainHallLower"]->FindObject("Bookshelf2");
	tmp->SetPuzzleName("BookshelfPuzzle2");
	tmp->Init(puzzleObj->getSize().x, puzzleObj->getSize().y, puzzleObj->getPos());
	tmp->SetTexture(puzzleObj->GetTexture());
	tmp->SetCurrentDialogueName("");
	g->GetCurrentLevel()->rooms["MainHallLower"]->objects.push_back(tmp);
	puzzleObj->Appear(false);
	GameScreen* gs = ((GameScreen*)g->GetScreen());
	gs->ClosePuzzle();
	Room* room = g->GetCurrentLevel()->rooms["EmmaRoom"];
	((InteractableObj*)room->FindObject("EmmaRoom_Drawing"))->SetCurrentDialogueName("EmmaRoom_picture2");
	((InteractableObj*)room->FindObject("EmmaRoom_Book"))->SetCurrentDialogueName("EmmaRoom_book2");
	((InteractableObj*)room->FindObject("EmmaRoom_Window"))->SetCurrentDialogueName("EmmaRoom_window2");


	for (auto npc : g->GetCurrentLevel()->rooms["MainHallLower"]->npcs) {
		((InteractableObj*)npc)->Appear(false);
	}
	for (auto npc : g->GetCurrentLevel()->rooms["MainHallUpper"]->npcs) {
		((InteractableObj*)npc)->Appear(false);
	}
	for (auto npc : g->GetCurrentLevel()->rooms["EmmaRoom"]->npcs) {
		((InteractableObj*)npc)->Appear(false);
	}
	gs->butler->Appear();
}