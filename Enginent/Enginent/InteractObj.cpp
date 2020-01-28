#include "InteractObj.h"
#include "Game.h"
#include "GameViewWindow.h"
#include "InfoPhone.h"

InteractableObj::InteractableObj(vector<std::string> s) {
	interactType = NORMAL;
	dialogue = s;
}

void InteractableObj::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
	/*std::cout << "collider min bound: " << col->getMinBound().x << ", " << col->getMinBound().y << std::endl;
	std::cout << "collider max bound: " << col->getMaxBound().x << ", " << col->getMaxBound().y << std::endl;*/
}

void InteractableObj::SetDialogue(vector<std::string> s) {
	dialogue = s;
}

void InteractableObj::action() {
	if (dialogue.size() > 0)
	{
		((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer()->SetDialogue(dialogue[currDialogue]);
		currDialogue = (++currDialogue) % dialogue.size();
	}
	if (takePic) {
		Phone::GetInstance()->AddPage(NOTE, picName);
		takePic = false;
	}
	//ANIMATION
	//UPDATE TEXT
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
	{
		return true;
	}
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

OpenObj::OpenObj() { 
	interactType = OPEN;
	open = false; 
	item = nullptr; 
}

void OpenObj::SetOpenTexture(std::string openT) {
	openTexture = Game::GetInstance()->GetRenderer()->LoadTexture(openT);
}

void OpenObj::SetNextTexture(std::string next) {
	nextTexture = Game::GetInstance()->GetRenderer()->LoadTexture(next);
}

void OpenObj::action() {
	if (!open) {
		open = true;
		SetTexture(openTexture);
		if (item)interactType = PICKUP;
		std::cout << "open\n";
	}
	else {
		if (item) {
			std::cout << "take\n";
			SetTexture(nextTexture);
			Game::GetInstance()->GetPlayer()->inventory->addItem(item);
			item = nullptr;
			interactType = NORMAL;
		}
	}
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
	// set description
}

void UseItemObj::SetItemToUse(std::string item_to_unlock) {
	this->item_to_use = item_to_unlock;
	used = false;
}