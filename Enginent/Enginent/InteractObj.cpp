#include "InteractObj.h"
#include "Game.h"
#include "GameWindows.h"
#include "InfoPhone.h"
#include "GameWindows.h"
#include "MouseInput.h"

InteractableObj::InteractableObj() {
	sound.clear();
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

void InteractableObj::Trigger() {
	triggered = true;
}

void InteractableObj::TakeNote() {
	//std::cout << (takeNote ? "yes" : "no") << "\n";
	if (takeNote) {
		interactType = NORMAL;
		col->enable = false;
		Phone* phone = Phone::GetInstance();
		for (int i = 0; i < noteNames.size(); i++) {
			phone->AddPage(NOTE, noteNames[i]);
		}
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
	SoundManager::GetInstance()->playSound(SFX,sound);
	if (talk == true)
	{
		if (dialogue_after != "")
			dialogue_name = dialogue_after;
	}
	if (dialogue_name != "")
	{
		TextBox::GetInstance()->setText(this->dialogue_name, talk);
		if (!talk)
		{
			talk = true;
		}
		TextBox::GetInstance()->SetDisplay(true);
	}
	if(interactType != DOOR)
		Game::GetInstance()->GetPlayer()->anim->Play("Idle");
	for (auto obj : triggerObjs)
		obj->Trigger();
	PickUpItem();
	TakeNote();
}

void InteractableObj::AddNoteName(std::string note) {
	noteNames.push_back(note);
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
	item->DialogueHandle(this->object_name);
	if (item != nullptr && item_to_use == item->name) {
		//std::cout << "item is used and removed from the inventory\n";
		GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
		Inventory* i = gs->GetInventory();
		used = true;
		if (hasPositionAfterUsed)
			SetPosition(positionAfterUse);
		if (hasUsedTexture)
			SetTexture(usedTexture);
		if (hasNewItemAfterUsed) {
			Item* item = gs->FindItem(new_item_after_used);
			i->AddItem(item);
			ViewWindow* vw = ViewWindow::GetInstance();
			vw->SetViewItem(item);
			vw->Open();
		}
		std::cout << "InteractableObj::UseItem " << item->name << " multiple use " << item->multipleUse << std::endl;
		if(!item->multipleUse)
			i->RemoveItem(item);
		i->UnselectItem();
		if (item->name == "Puzzle6_PillBedtime_withMEAT")
			item->multipleUse = false;
	}
}
void InteractableObj::Used() {
	used = true;
	if (hasUsedTexture)
		SetTexture(usedTexture);
	if (hasPositionAfterUsed)
		SetPosition(positionAfterUse);
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
void InteractableObj::SetUsedTexture(std::string used) {
	usedTexture = Game::GetInstance()->GetRenderer()->LoadTexture(used);
	hasUsedTexture = true;
}
void InteractableObj::SetPositionAfterUsed(float x, float y) {
	positionAfterUse = glm::vec3(x, y, 1);
	hasPositionAfterUsed = true;
}
void InteractableObj::SetNewItemAfterUsed(std::string item) {
	new_item_after_used = item;
	hasNewItemAfterUsed = true;
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
		if (talk == true)
		{
			if (dialogue_after != "")
				dialogue_name = dialogue_after;
		}
		if (dialogue_name != "")
		{
			TextBox::GetInstance()->setText(this->dialogue_name, talk);
			if (!talk)
			{
				talk = true;
			}
			TextBox::GetInstance()->SetDisplay(true);
		}
	}
}

void InteractableObj::SetSound(std::string s) {
	sound = s;
}

void OpenObj::Open() {
	open = true;
	//std::cout << "open\n";
	if (talk == true)
	{
		if (dialogue_after != "")
			dialogue_name = dialogue_after;
	}
	if (dialogue_name != "")
	{
		TextBox::GetInstance()->setText(this->dialogue_name, talk);
		if (!talk)
		{
			talk = true;
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
NonPlayer::NonPlayer(std::string name) { 
	object_name = name; 
	interactType = TALK; 
	talk = false; 
}
void NonPlayer::action()
{
	Phone::GetInstance()->AddNPCInteracted(object_name);
	//std::cout << interactType << std::endl;
	if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
		UseItem(gs->GetInventory()->GetSelectedItem());
		dialogue_name = dialogue_after_use;
	}
	else {
		if (talk == true)
		{
			if (dialogue_after != "")
				dialogue_name = dialogue_after;
		}
		if (dialogue_name != "")
		{
			TextBox::GetInstance()->setText(this->dialogue_name, talk);
			if (!talk)
			{
				talk = true;
			}
			TextBox::GetInstance()->SetDisplay(true);
			if (dialogue_name == "Floor2_Mask4_2" || dialogue_name == "Floor2_Mask7_2"
				|| dialogue_name == "Floor2_Mask5_2" || dialogue_name == "Floor2_Mask6_2")
				Game::GetInstance()->GetPlayer()->TriggerRouteB++;
		}
	}
	Game::GetInstance()->GetPlayer()->anim->Play("Idle");
	if (object_name == "Emma") {
		((Door*)Game::GetInstance()->GetCurrentLevel()->FindObject("BackDoorRoom_BasementDoor"))->Trigger();
	}
	for (auto obj : triggerObjs)
		obj->Trigger();
	TakeNote();
}

void PuzzleObj::SetPuzzleName(std::string name) {
	puzzleName = name;
}

void PuzzleObj::action() {
	GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
	Puzzle* p = gs->FindPuzzle(puzzleName);
	//std::cout << " PuzzleObj::action() used " << used << std::endl;
	if (MouseInput::GetInstance()->GetActionEvent() == ITEM_SELECTED_ACTION) {
		UseItem(gs->GetInventory()->GetSelectedItem());
		if (used) {
			dialogue_name = dialogue_after_use;
			dialogue_after_use.clear();
		}
	}
	else if (p && used && gs->puzzles[puzzleName]->CheckRequirements()) {
		if (dialogue_name != "") {
			TextBox::GetInstance()->setText(dialogue_name);
			dialogue_name = "";
		}
		else {
			//Puzzle* p = gs->puzzles[puzzleName];
			//TextBox::GetInstance()->setText(p->prepTalk);
			gs->OpenPuzzle(puzzleName);
		}
	}
	else {
		InteractableObj::action();
	}
}

void SaveObj::action() {
	Game::GetInstance()->SetSaveGame(true);
	SaveLoadWindow::GetInstance()->Open();
}
PlayerTriggerObj::PlayerTriggerObj() {
	triggered = false;
}
void PlayerTriggerObj::Update() {
	if (anim && display)
		anim->Update();
	if (!triggered && col->isCollide(Game::GetInstance()->GetPlayer()->col)) {
		action();
		triggered = true;
		Game::GetInstance()->GetPlayer()->StopWalking();
	}
	if (triggered && !TextBox::GetInstance()->IsDisplay()) {
		display = false;
		col->enable = false;
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
			UnlockBookshelf();
		}
		
	}
	else if (!used) {
		InteractableObj::action();
	}
	else {
		TextBox::GetInstance()->setText("backdoor_card");
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
	tmp->ChangeDialogue("Hall_Bookshelf_R1", "Hall_Bookshelf_R1");
	tmp->SetItemToUse("keyCard");
	tmp->SetName("bookShelfPuzzle2");
	g->GetCurrentLevel()->rooms["MainHallLower"]->objects.push_back(tmp);
	puzzleObj->Appear(false);
	GameScreen* gs = ((GameScreen*)g->GetScreen());
	gs->ClosePuzzle();
	Room* room = g->GetCurrentLevel()->rooms["EmmaRoom"];
	((InteractableObj*)room->FindObject("EmmaRoom_Drawing"))->ChangeDialogue("EmmaRoom_picture2", "EmmaRoom_picture2");
	((InteractableObj*)room->FindObject("EmmaRoom_Book"))->ChangeDialogue("EmmaRoom_book2","EmmaRoom_book2");
	((InteractableObj*)room->FindObject("EmmaRoom_Window"))->ChangeDialogue("EmmaRoom_window2", "EmmaRoom_window2");
	InteractableObj* emmaNote = ((InteractableObj*)room->FindObject("EmmaRoom_Note1"));
	emmaNote->ChangeDialogue("EmmaRoom_note", "EmmaRoom_note");
	emmaNote->SetInteractType(ADDNOTE);
	emmaNote->col->enable = true;
	Item* i = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory()->FindItem("keyCard");
	if(i)
		i->multipleUse = false;

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
RemoveObj::RemoveObj() {

}
void RemoveObj::action() {
	InteractableObj::action();
	if (!used) 
		RemoveSelf();
}

void RemoveObj::RemoveSelf() {
	used = true;
	col->enable = false;
	display = false;
}

void RemoveObj::Trigger() {
	triggered = true;
	col->enable = true;
}
