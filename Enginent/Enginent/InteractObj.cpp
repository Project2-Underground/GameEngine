#include "InteractObj.h"
#include "Game.h"
#include "GameWindows.h"
#include "InfoPhone.h"
#include "GameWindows.h"

InteractableObj::InteractableObj(vector<Dialogue> s) {
	interactType = NORMAL;
	dialogue = s;
}

void InteractableObj::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
	/*std::cout << "collider min bound: " << col->getMinBound().x << ", " << col->getMinBound().y << std::endl;
	std::cout << "collider max bound: " << col->getMaxBound().x << ", " << col->getMaxBound().y << std::endl;*/
}

void InteractableObj::SetDialogue(vector<Dialogue> s) {
	dialogue = s;
}

void InteractableObj::TakePic() {
	if (takePic) {
		Phone* phone = Phone::GetInstance();
		phone->AddPage(NOTE, picName);
		phone->SetNotification(NOTE);
		takePic = false;
	}
}

void InteractableObj::action() {
	if (dialogue.size() > 0)
	{
		//((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer()->SetDialogue(dialogue[currDialogue]);
		TextBox::GetInstance()->setText(dialogue[currDialogue]);
		TextBox::GetInstance()->SetDisplay(true);
		currDialogue = (++currDialogue) % dialogue.size();
	}
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
	if (item) {
		GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
		gs->GetInventory()->AddItem(item);
		gs->GetPlayer()->anim->Play("Pickup", false);

		ViewWindow* vw = ViewWindow::GetInstance();
		vw->Open();
		vw->SetViewItem(item->GetViewTexture());

		item = nullptr;
		interactType = NORMAL;
	}
}

OpenObj::OpenObj() {
	interactType = PICKUP;
	open = false;
	item = nullptr;
}

void OpenObj::SetOpenTexture(std::string openT) {
	openTexture = Game::GetInstance()->GetRenderer()->LoadTexture(openT);
	nextTexture = Game::GetInstance()->GetRenderer()->LoadTexture(openT);
}

void OpenObj::SetNextTexture(std::string next) {
	nextTexture = Game::GetInstance()->GetRenderer()->LoadTexture(next);
}

void OpenObj::action() {
	if (!open) {
		Open();
	}
	else {
		PickUpItem();
		SetTexture(nextTexture);
	}
	TakePic();
}

void OpenObj::Open() {
	open = true;
	SetTexture(openTexture);
	if (item)interactType = PICKUP;
	else interactType = NORMAL;
}

void OpenObj::ClearItem() {
	item = nullptr;
	interactType = NORMAL;
	SetTexture(nextTexture);
}

OpenObj::~OpenObj() {
	if (item)
		delete item;
}

void ViewObj::SetViewTexture(std::string view) {
	viewTexture = Game::GetInstance()->GetRenderer()->LoadTexture(view);
}

void ViewObj::action() {
	ViewWindow* vw = ViewWindow::GetInstance();

	vw->Open();
	vw->SetViewItem(viewTexture);
	TakePic();
	// set description
}

void PuzzleObj::SetPuzzleName(std::string name) {
	puzzleName = name;
}

void PuzzleObj::action() {
	((GameScreen*)Game::GetInstance()->GetScreen())->OpenPuzzle(puzzleName);
}

void SaveObj::action() {
	Game::GetInstance()->SetSaveGame(true);
	SaveLoadWindow::GetInstance()->Open();
}