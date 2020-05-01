
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
	play = new SwitchScene_Button("Texture/UI/MainScreen/MainScreen_Play.png", "Texture/UI/MainScreen/StartBotton_Point.png", "Texture/UI/MainScreen/StartBotton_Click.png");
	play->SetSize(248, -70);
	play->SetPosition(glm::vec3(-270, 170, 1));
	play->SetCollder(new Collider(play));

	load = new OpenLoadSaveWindow("Texture/UI/MainScreen/MainScreen_Load.png");
	load->SetHoverTexture("Texture/tmp_texture/tmp_loadButtonPress.png");
	load->SetSize(213, -74);
	load->SetPosition(glm::vec3(-270, 90, 1));
	load->SetCollder(new Collider(load));

	setting = new SettingWindowCloseButton("Texture/UI/MainScreen/MainScreen_Sound.png");
	setting->SetSize(264, -81);
	setting->SetPosition(glm::vec3(-270.0f, 20, 1.0f));
	setting->SetCollder(new Collider(setting));

	quit = new Exit_Button("Texture/UI/MainScreen/MainScreen_Ouit.png", "Texture/UI/MainScreen/ExitBotton_Point.png", "Texture/UI/MainScreen/ExitBotton_Click.png");;
	quit->SetSize(186, -86);
	quit->SetPosition(glm::vec3(-270.0f, -50.0f, 1.0f));
	quit->SetCollder(new Collider(quit));

	background = new UIObject();
	background->SetTexture("Texture/UI/MainScreen/MainScreen_Template.png");
	background->SetSize(1280, -720);
	background->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));

	UIObject* title = new UIObject();
	title->SetTexture("Texture/UI/MainScreen/MainScreen_Undergroung.png");
	title->SetSize(756.0f, -94.0f);
	title->SetPosition(glm::vec3(-270.0f, 250.0f, 1.0f));

	UI.push_back(background);
	UI.push_back(title);
	UI.push_back(play);
	UI.push_back(setting);
	UI.push_back(quit);
	UI.push_back(load);

	windows.push_back(SettingWindow::GetInstance());
	windows.push_back(SaveLoadWindow::GetInstance());
	SoundManager::GetInstance()->playSound(BGM, "MainScreen", true);
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
	levels.push_back("save/level2.xml");
	levels.push_back("save/level3.xml");
	objActions.push_back("save/objSpecialAction1.xml");
	objActions.push_back("save/objSpecialAction2.xml");
	objActions.push_back("save/objSpecialAction3.xml");

	player = new Player();
	butler = new Butler();
	currentLevel = new Level(levels[0]);
	XMLManager::GetInstance()->LoadItems(items);
	XMLManager::GetInstance()->LoadObjSpecialActions(objActions[0], currentLevel);

	//player->SetTexture("Texture/Character/Elias_idle.png");
	player->SetSize(205.0f, -430.0f);
	player->SetPosition(glm::vec3(0.0f, -50.0f, 1.0f));
	player->SetCollder(new Collider(player));
	player->SetWalkLimit(currentLevel->GetCurrentRoom()->GetPlayerWalkLimit());

	Camera* camera = Game::GetInstance()->GetCamera();
	camera->SetTarget(player);
	camera->SetLimit(currentLevel->GetCurrentRoom()->GetCameraLimit());

	inventory = new Inventory();
	phone = Phone::GetInstance();
	phoneIcon = new PhoneOpenButton("Texture/tmp_texture/tmp_phone.png");
	phoneIcon->Init(100.0f, -100.0f, glm::vec3(-500.0f, 300.0f, 1.0f));
	phoneIcon->Disappear();
	XMLManager::GetInstance()->LoadNotes("save/notes.xml", phone->notes);
	XMLManager::GetInstance()->LoadChats("save/chats.xml", phone->chats);

	puzzles.insert(std::pair<std::string, Puzzle*>("BookshelfPuzzle", new BookshelfPuzzle()));
	PuzzleTime = false;

	dialogueText = TextBox::GetInstance();

	pause = new OpenPauseWindowButton("Texture/tmp_texture/tmp_pause.png");
	pause->SetSize(60, -60);
	pause->SetPosition(glm::vec3(600, 300, 1));
	pause->SetCollder(new Collider(pause));

	UI.push_back(phoneIcon);
	UI.push_back(pause);

	windows.push_back(ViewWindow::GetInstance());
	windows.push_back(SaveLoadWindow::GetInstance());
	//windows.push_back(SettingWindow::GetInstance());
	windows.push_back(PauseWindow::GetInstance());
}

void GameScreen::LoadGame(std::string filename) {
	XMLManager::GetInstance()->LoadFromSave(filename);
}

Item* GameScreen::FindItem(std::string name) {
	std::cout << items.size();
	for (auto i : items)
		if (i->name == name)
			return i;
	return nullptr;
}

void GameScreen::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();

	if (PuzzleTime) 
		currentPuzzle->Render();
	else {
		currentLevel->Render();
		renderer->Render(butler);
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

	dialogueText->Render();
}

void GameScreen::Update() {
	for (auto w : windows)
		w->Update();
	if (!Pause) {
		if (!dialogueText->IsDisplay()) {
			if (PuzzleTime)
				currentPuzzle->Update();
			else {
				currentLevel->Update();
				player->Update();
				butler->Update();
			}
			if (InventoryEnable && !phone->open)
				inventory->Update();
		}
	}
}

void GameScreen::RightClick(glm::vec3 screen, glm::vec3 world) {
	if (!PuzzleTime) {
		inventory->UnselectItem();
		if (!phone->open && !player->anim->IsPlaying("Pickup") && !GameWindowOpen() && !dialogueText->IsDisplay())
			currentLevel->RightClick(world.x, world.y);
	}
	else if (dialogueText->IsDisplay())
		dialogueText->clickLeft(screen);
}

void GameScreen::LeftClick(glm::vec3 screen, glm::vec3 world) {
	if (dialogueText->IsDisplay())
		dialogueText->clickLeft(screen);
	else if (GameWindowOpen()) {
		for (auto w : windows)
			w->LeftClick(screen.x, screen.y);
	}
	else if (phone->open) 
		phone->LeftClick(screen.x, screen.y);
	else if (PuzzleTime)
		currentPuzzle->LeftClick(screen, world);
	else {
		buttonClicked = false;
		for (int j = 0; j < UI.size(); j++)
			if (Button * button = dynamic_cast<Button*>(UI[j]))
				button->checkColliderPressed(screen.x, screen.y);
		if(!buttonClicked && !player->anim->IsPlaying("Pickup"))
			currentLevel->LeftClick(world.x, world.y);
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
	XMLManager::GetInstance()->LoadObjSpecialActions(objActions[level], currentLevel);
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

Door* GameScreen::GetDoor(std::string doorNam) {
	for (auto room : currentLevel->rooms) {
		for (auto door : room.second->doors)
			if (door.second->object_name == doorNam)
				return door.second;
	}
	return nullptr;
}

void GameScreen::OpenPuzzle(std::string name) {
	player->StopWalking();
	currentPuzzle = puzzles[name];
	PuzzleTime = true;
	InventoryEnable = currentPuzzle->IsInventoryEnable();
	Game::GetInstance()->GetCursor()->enableChange(false);
}

void GameScreen::ResetPuzzle() {
	for (auto puzzle : puzzles)
		puzzle.second->Reset();
}

void GameScreen::ClosePuzzle() {
	PuzzleTime = false;
	InventoryEnable = true;
	Game::GetInstance()->GetCursor()->enableChange(true);
}

void GameScreen::HandleKey(SDL_Keycode key) {
	switch (key)
	{
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
	case SDLK_1:
		puzzles["BookshelfPuzzle"]->CompletePuzzle();
		break;
	default:
		break;
	}
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

/*TestScene NumPad*/

TestScreen::TestScreen()
{
	numpad = new NumpadPuzzle_2();
}

void TestScreen::Init()
{

}

void TestScreen::Render()
{
	numpad->Render();
}

void TestScreen::Update()
{
	numpad->Update();
}

void TestScreen::RightClick(glm::vec3, glm::vec3)
{

}

void TestScreen::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	numpad->LeftClick(screen, world);
}

void TestScreen::UpdateMouseState(glm::vec3 screen, glm::vec3 world)
{

}

TestScreen::~TestScreen()
{

}

void TestScreen::RightRelease(glm::vec3, glm::vec3)
{

}

void TestScreen::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	numpad->LeftRelease(screen, world);
}

void TestScreen::HandleKey(SDL_Keycode)
{

}