#include "Screen.h"
#include "Game.h"

/*MAIN MENU*/

MenuScreen::MenuScreen() {
	play = new SwitchScene_Button("Texture/UI/MainScreen/StartBotton_Normal.png", "Texture/UI/MainScreen/StartBotton_Point.png", "Texture/UI/MainScreen/StartBotton_Click.png");
	play->SetSize(300, -120);
	play->SetPosition(glm::vec3(-170.0f, 70.0f, 1.0f));
	Collider* col = new Collider(play);

	play->SetCollder(col);
	// setting button
	setting;
	// load button
	load;

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


}

void MenuScreen::Render() {
	Game::GetInstance()->GetRenderer()->Render(UI);
}

void MenuScreen::Update() {

}

void MenuScreen::LeftClick(glm::vec3 screen, glm::vec3 world) {
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkCollider(screen.x, screen.y);
		}
	}
}

void MenuScreen::RightClick(glm::vec3, glm::vec3)
{

}

void MenuScreen::UpdateMouseState(glm::vec3 screen, glm::vec3 world) {

	play->updateButton(screen.x, screen.y);
	//setting->updateButton(realPos.x, realPos.y);
	quit->updateButton(screen.x, screen.y);
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
	player->SetSize(140.0f, -430.0f);
	player->SetPosition(glm::vec3(0.0f, -50.0f, 1.0f));
	player->SetCollder(new Collider(player));
	player->SetWalkLimit(currentLevel->GetCurrentRoom()->GetPlayerWalkLimit());

	Camera* camera = Game::GetInstance()->GetCamera();
	camera->SetTarget(player);
	camera->SetLimit(currentLevel->GetCurrentRoom()->GetCameraLimit());

	Game* g = Game::GetInstance();
	viewWin = ViewWindow::GetInstance();
	viewWin->Init(g->winWidth, g->winHeight);

	inventory = new Inventory();
	phone = Phone::GetInstance();
	phoneIcon = new PhoneOpenButton("Texture/tmp_phone.png");
	phoneIcon->SetSize(100.0f, -100.0f);
	phoneIcon->SetPosition(glm::vec3(-500.0f, 300.0f, 1.0f));
	phoneIcon->SetCollder(new Collider(phoneIcon));

	puzzles.insert(std::pair<std::string, Puzzle*>("BookshelfPuzzle", new BookshelfPuzzle()));
	PuzzleTime = false;

	dialogueText = TextBox::GetInstance();

	UI.push_back(phoneIcon);
}

void GameScreen::LoadGame(std::string filename) {
	XMLManager::GetInstance()->LoadFromSave(filename);
}

void GameScreen::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();

	if (PuzzleTime) {
		currentPuzzle->Render();
	}else{
		currentLevel->Render();
		renderer->Render(player);
		renderer->Render(currentLevel->GetCurrentRoom()->foreground, false);
	}
	renderer->Render(UI);

	inventory->Render();
	viewWin->Render();
	if (phone->open)
		phone->Render();
	if (dialogueText->IsDisplay())
		dialogueText->Render();
}

void GameScreen::Update() {
	if (PuzzleTime)
		currentPuzzle->Update();
	else {
		currentLevel->Update();
		player->Update();
	}
	if(InventoryEnable)
		inventory->Update();
}

void GameScreen::RightClick(glm::vec3 screen, glm::vec3 world) {
	if (!PuzzleTime) {
		inventory->UnselectItem();
		if (!phone->open)
			currentLevel->RightClick(world.x, world.y);
	}
}

void GameScreen::LeftClick(glm::vec3 screen, glm::vec3 world) {
	if (phone->open)
		phone->LeftClick(screen.x, screen.y);
	else if (dialogueText->IsDisplay() == true)
		dialogueText->SetDisplay(false);
	else if (PuzzleTime) 
		currentPuzzle->LeftClick(screen, world);
	else if (viewWin->IsOpen()) 
		viewWin->LeftClick(screen.x, screen.y);
	else {
		currentLevel->LeftClick(world.x, world.y);
	}
	if (InventoryEnable) {
		inventory->LeftClick(screen.x, screen.y);
	}
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkCollider(screen.x, screen.y);
		}
	}
}

void GameScreen::UpdateMouseState(glm::vec3, glm::vec3)
{

}

void GameScreen::ChangeLevel(int level) {
	delete currentLevel;
	currentLevel = new Level(levels[level]);
}

void GameScreen::ChangeRoom(std::string room, std::string door) {
	currentLevel->ChangeRoom(room, door);
}

int GameScreen::GetPointedObject(glm::vec3 pos) {
	std::vector<DrawableObject*>* objects = currentLevel->Getobjects();
	for (int i = (int)objects->size() - 1; i >= 0; i--)
	{
		if (dynamic_cast<InteractableObj*>((*objects)[i]) && dynamic_cast<InteractableObj*>((*objects)[i])->CheckPointing(pos.x, pos.y))
		{
			return dynamic_cast<InteractableObj*>((*objects)[i])->getType();
		}
	}
	return (int)NORMAL;
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

void CutsceneScreen::UpdateMouseState(glm::vec3, glm::vec3)
{

}

void CutsceneScreen::HandleKey(SDL_Keycode key) {

}