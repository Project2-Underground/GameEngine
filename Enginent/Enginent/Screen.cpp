#include "Screen.h"
#include "Game.h"



bool Screen::GameWindowOpen() {
	for (auto w : windows)
		if (w->IsOpen())
			return true;
	return false;
}

void Screen::CloseGameAllWindow() {
	for (auto w : windows)
		w->Close();
}

/*MAIN MENU*/
MenuScreen::MenuScreen() {
	play = new SwitchScene_Button("Texture/UI/MainScreen/StartBotton_Normal.png", "Texture/UI/MainScreen/StartBotton_Point.png", "Texture/UI/MainScreen/StartBotton_Click.png");
	play->SetSize(300, -120);
	play->SetPosition(glm::vec3(-170.0f, 70.0f, 1.0f));
	play->SetCollder(new Collider(play));

	// setting button
	setting;

	load = new OpenLoadSaveWindow("Texture/tmp_texture/tmp_loadButton.png");
	load->SetHoverTexture("Texture/tmp_texture/tmp_loadButtonPress.png");
	load->SetSize(300, -120);
	load->SetPosition(glm::vec3(170.0f, -50.0f, 1.0f));
	load->SetCollder(new Collider(load));

	quit = new Exit_Button("Texture/UI/MainScreen/ExitBotton_Normal.png", "Texture/UI/MainScreen/ExitBotton_Point.png", "Texture/UI/MainScreen/ExitBotton_Click.png");;
	quit->SetSize(300, -120);
	quit->SetPosition(glm::vec3(-170.0f, -50.0f, 1.0f));
	quit->SetCollder(new Collider(quit));

	background = new UIObject();
	background->SetTexture("Texture/UI/MainScreen/MainScreen_Click.png");
	background->SetSize(1280, -720);
	background->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));

	UI.push_back(background);
	UI.push_back(play);
	UI.push_back(quit);
	UI.push_back(load);

	windows.push_back(SettingWindow::GetInstance());
	windows.push_back(SaveLoadWindow::GetInstance());
}

void MenuScreen::Render() {
	Game::GetInstance()->GetRenderer()->Render(UI);
	for (auto w : windows)
		w->Render();
}

void MenuScreen::Update() {
	for (auto w : windows)
		w->Update();
}

void MenuScreen::LeftClick(glm::vec3 screen, glm::vec3 world) {
	if(GameWindowOpen())
		for (auto w : windows)
			w->LeftClick(screen.x, screen.y);
	else 
		for (int j = 0; j < UI.size(); j++)
			if (Button * button = dynamic_cast<Button*>(UI[j]))
				button->checkColliderPressed(screen.x, screen.y);
}

void MenuScreen::RightClick(glm::vec3, glm::vec3)
{

}

void MenuScreen::RightRelease(glm::vec3 screen, glm::vec3 world)
{

}

void MenuScreen::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	if (GameWindowOpen())
		for (auto w : windows)
			w->LeftRelease(screen.x, screen.y);
	else
		for (int j = 0; j < UI.size(); j++)
			if (Button * button = dynamic_cast<Button*>(UI[j]))
				button->checkColliderReleased(screen.x, screen.y);
}

void MenuScreen::UpdateMouseState(glm::vec3 screen, glm::vec3 world) {
	play->updateButton(screen.x, screen.y);
	//setting->updateButton(realPos.x, realPos.y);
	quit->updateButton(screen.x, screen.y);
	if(GameWindowOpen())
		for(auto w:windows)
			if (w->IsOpen()) {
				w->UpdateMouseButton(screen);
				break;
			}
}

void MenuScreen::HandleKey(SDL_Keycode key) {

}

MenuScreen::~MenuScreen() {
	delete play;
	//delete load;
	//delete setting;
	delete quit;
	delete background;

	for (auto ui : UI)
		delete ui;
}

/*GAME SCREEN*/
GameScreen::GameScreen() {
	InventoryEnable = true;
	PuzzleTime = false;

	// filepath of levels
	levels.push_back("save/level1.xml");
	//levels.push_back("save/level2.xml");
	//levels.push_back("save/level3.xml");

	currentLevel = new Level(levels[0]);
	player = new Player();
	//player->SetTexture("Texture/Character/Elias_idle.png");
	player->SetSize(205.0f, -430.0f);
	player->SetPosition(glm::vec3(0.0f, -50.0f, 1.0f));
	player->SetCollder(new Collider(player));
	player->SetWalkLimit(currentLevel->GetCurrentRoom()->GetPlayerWalkLimit());

	Camera* camera = Game::GetInstance()->GetCamera();
	camera->SetTarget(player);
	camera->SetLimit(currentLevel->GetCurrentRoom()->GetCameraLimit());

	Game* g = Game::GetInstance();

	inventory = new Inventory();
	phone = Phone::GetInstance();
	phoneIcon = new PhoneOpenButton("Texture/tmp_texture/tmp_phone.png");
	phoneIcon->SetSize(100.0f, -100.0f);
	phoneIcon->SetPosition(glm::vec3(-500.0f, 300.0f, 1.0f));
	phoneIcon->SetCollder(new Collider(phoneIcon));
	XMLManager::GetInstance()->LoadNotes("save/notes.xml", phone->notes);
	XMLManager::GetInstance()->LoadChats("save/chats.xml", phone->chats);
	// test
	phone->AddPage(NOTE, "tmp_note1");
	phone->AddPage(NOTE, "tmp_note2");
	phone->AddPage(CHAT, "person_name");
	phone->Message("person_name", 2);
	phone->SetNotification(NOTE);
	phone->SetNotification(CHAT);
	// test

	puzzles.insert(std::pair<std::string, Puzzle*>("BookshelfPuzzle", new BookshelfPuzzle()));
	PuzzleTime = false;

	dialogueText = TextBox::GetInstance();

	pause = new OpenPauseWindowButton("");
	pause->SetSize(60, -60);
	pause->SetPosition(glm::vec3(600, 300, 1));
	pause->SetCollder(new Collider(pause));

	UI.push_back(phoneIcon);
	UI.push_back(pause);

	windows.push_back(ViewWindow::GetInstance());
	windows.push_back(SaveLoadWindow::GetInstance());
	windows.push_back(SettingWindow::GetInstance());
	windows.push_back(PauseWindow::GetInstance());
}

void GameScreen::LoadGame(std::string filename) {
	XMLManager::GetInstance()->LoadFromSave(filename);
}

void GameScreen::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();

	if (PuzzleTime) 
		currentPuzzle->Render();
	else {
		currentLevel->Render();
	}
	renderer->Render(UI);

	inventory->Render();
	if(GameWindowOpen())
		for (auto w : windows)
			w->Render();
	else {
		if (phone->open)
			phone->Render();
		if (dialogueText->IsDisplay())
			dialogueText->Render();
	}
}

void GameScreen::Update() {
	for (auto w : windows)
		w->Update();
	if (!Pause) {
		if (PuzzleTime)
			currentPuzzle->Update();
		else {
			currentLevel->Update();
			player->Update();
		}
		if (InventoryEnable && !phone->open)
			inventory->Update();
	}
}

void GameScreen::RightClick(glm::vec3 screen, glm::vec3 world) {
	if (!PuzzleTime) {
		inventory->UnselectItem();
		if (!phone->open && !player->anim->IsPlaying("Pickup") && !GameWindowOpen() && !dialogueText->IsDisplay())
			currentLevel->RightClick(world.x, world.y);
		else if(dialogueText->IsDisplay())
			dialogueText->SetDisplay(false);
	}
}

void GameScreen::LeftClick(glm::vec3 screen, glm::vec3 world) {
	if(GameWindowOpen())
		for (auto w : windows)
			w->LeftClick(screen.x, screen.y);
	else if (phone->open)
		phone->LeftClick(screen.x, screen.y);
	else if (dialogueText->IsDisplay())
		dialogueText->SetDisplay(false);
	else if (PuzzleTime)
		currentPuzzle->LeftClick(screen, world);
	else {
		if(!player->anim->IsPlaying("Pickup"))
			currentLevel->LeftClick(world.x, world.y);
		for (int j = 0; j < UI.size(); j++)
			if (Button * button = dynamic_cast<Button*>(UI[j]))
				button->checkColliderPressed(screen.x, screen.y);
	}
	if (InventoryEnable) {
		inventory->LeftClick(screen.x, screen.y);
	}
}

void GameScreen::RightRelease(glm::vec3 screen, glm::vec3 world)
{

}

void GameScreen::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	if (GameWindowOpen())
		for (auto w : windows)
			w->LeftRelease(screen.x, screen.y);
	else if (phone->open)
		phone->LeftRelease(screen.x, screen.y);
	else if (PuzzleTime)
		currentPuzzle->LeftRelease(screen, world);
	if (InventoryEnable) {
		inventory->LeftRelease(screen.x, screen.y);
	}
	for (int j = 0; j < UI.size(); j++)
		if (Button * button = dynamic_cast<Button*>(UI[j]))
			button->checkColliderReleased(screen.x, screen.y);
}

void GameScreen::Scroll(glm::vec3 screen, int direction) {
	if (phone->open)
		phone->Scroll(screen, direction);
}

void GameScreen::UpdateMouseState(glm::vec3 screen, glm::vec3 world)
{
	if (GameWindowOpen())
		for (auto w : windows)
			if (w->IsOpen()) {
				w->UpdateMouseButton(screen);
				break;
			}
}

void GameScreen::ChangeLevel(int level) {
	if(currentLevel)
		delete currentLevel;
	currentLevel = new Level(levels[level]);
}

void GameScreen::ChangeRoom(std::string room, std::string door) {
	currentLevel->ChangeRoom(room, door);
}

InteractTypeList GameScreen::GetPointedObject(glm::vec3 pos) {
	std::vector<DrawableObject*>* objects = currentLevel->Getobjects();
	for (int i = (int)objects->size() - 1; i >= 0; i--)
	{
		if (InteractableObj* obj = dynamic_cast<InteractableObj*>((*objects)[i]))
			if(obj->CheckPointing(pos.x, pos.y))
			{
				//std::cout << obj->object_name << ", " << obj->getType() <<  std::endl;
				return obj->getType();
			}
	}
	return NORMAL;
}

void GameScreen::OpenPuzzle(std::string name) {
	player->StopWalking();
	currentPuzzle = puzzles[name];
	PuzzleTime = true;
	InventoryEnable = currentPuzzle->IsInventoryEnable();
}


void GameScreen::ClosePuzzle() {
	PuzzleTime = false;
	InventoryEnable = true;
}

void GameScreen::HandleKey(SDL_Keycode key) {
	//switch (key)
	//{
	//case SDLK_d: {
	//	// unlock door
	//	Item* item = inventory->GetInventoryBox(0)->GetItem();
	//	if (item != nullptr)
	//		currentLevel->GetCurrentRoom()->doors["EliasRoomInnerDoor"]->UseItem(item);
	//}break;
	//case SDLK_w: {
	//	Item* item = inventory->GetInventoryBox(0)->GetItem();
	//	if (item != nullptr && dynamic_cast<SeparatableItem*>(item)) 
	//		((SeparatableItem*)item)->action();
	//}break;
	//case SDLK_s:
	//	// save current game
	//	XMLManager::GetInstance()->SaveGame("save/test.xml");
	//	break;
	//case SDLK_l:
	//	XMLManager::GetInstance()->LoadFromSave("save/test.xml");
	//	break;
	//case SDLK_p:
	//	Phone::GetInstance()->open = !Phone::GetInstance()->open;
	//	break;
	//default:
	//	break;
	//}
}

GameScreen::~GameScreen() {
	delete player;
	delete phone;
	//delete pause;
	delete currentLevel;

	for (auto p : puzzles)
		delete p.second;

	for (auto ui : UI)
		delete ui;

	delete inventory;
}

/*CUTSCENE*/
CutsceneScreen::CutsceneScreen() {

}

void CutsceneScreen::Render() {
	// play the cutscene
}
void CutsceneScreen::Update() {
	// eg. 2 clicks to skip a cutscene
}

void CutsceneScreen::LeftClick(glm::vec3, glm::vec3) {

}

void CutsceneScreen::RightClick(glm::vec3, glm::vec3)
{

}

void CutsceneScreen::RightRelease(glm::vec3, glm::vec3)
{

}

void CutsceneScreen::LeftRelease(glm::vec3, glm::vec3)
{

}

void CutsceneScreen::UpdateMouseState(glm::vec3, glm::vec3)
{

}

void CutsceneScreen::HandleKey(SDL_Keycode key) {

}