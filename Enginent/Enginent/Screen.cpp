
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
		w->ForceClose();
}

/*MAIN MENU*/
MenuScreen::MenuScreen() {
	play = new SwitchScene_Button("Texture/UI/MainScreen/MainScreen_Play.png", "Texture/UI/MainScreen/StartBotton_Point.png", "Texture/UI/MainScreen/StartBotton_Click.png");
	play->Init(248, -70, glm::vec3(-258, 78, 1));

	load = new OpenLoadSaveWindow("Texture/UI/MainScreen/MainScreen_Load.png");
	load->SetHoverTexture("Texture/tmp_texture/tmp_loadButtonPress.png");
	load->Init(213, -74, glm::vec3(-277, -18, 1));

	setting = new SettingButton("Texture/UI/MainScreen/MainScreen_Sound.png");
	setting->Init(264, -81, glm::vec3(-266, -111, 1.0f));

	quit = new Exit_Button("Texture/UI/MainScreen/MainScreen_Ouit.png", "Texture/UI/MainScreen/ExitBotton_Point.png", "Texture/UI/MainScreen/ExitBotton_Click.png");;
	quit->Init(186, -86, glm::vec3(-262, -211, 1.0f));

	background = new UIObject();
	background->SetTexture("Texture/UI/MainScreen/MainScreen_Template.png");
	background->SetSize(1280, -720);
	background->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));

	UIObject* title = new UIObject();
	title->SetTexture("Texture/UI/MainScreen/MainScreen_Undergroung.png");
	title->SetSize(756.0f, -94.0f);
	title->SetPosition(glm::vec3(-258, 207.0f, 1.0f));

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
	Game::GetInstance()->GetCursor()->ResetCursor();
	InventoryEnable = true;
	PuzzleTime = false;
	currentLevel = nullptr;
	// filepath of levels
	levels.push_back("save/level1.xml");
	levels.push_back("save/level2.xml");
	levels.push_back("save/level3.xml");
	objActions.push_back("save/objSpecialAction1.xml");
	objActions.push_back("save/objSpecialAction2.xml");
	objActions.push_back("save/objSpecialAction3.xml");
}

void GameScreen::Init() {
	dialogueText = TextBox::GetInstance();

	player = new Player();
	player->SetSize(205.0f, -430.0f);
	player->SetCollder(new Collider(player));

	Camera* camera = Game::GetInstance()->GetCamera();
	camera->SetTarget(player);

	butler = new Butler();

	ChangeLevel(0);
	XMLManager::GetInstance()->LoadItems(items);
	XMLManager::GetInstance()->LoadObjSpecialActions(objActions[0], currentLevel);

	inventory = new Inventory();
	phone = Phone::GetInstance();
	phone->Clear();
	phoneIcon = new PhoneOpenButton("Texture/UI/InfoPhone/Tablet_Mini.png");
	phoneIcon->Init(100.0f, -100.0f, glm::vec3(-580.0f, 300.0f, 1.0f));
	phoneIcon->Disappear();
	XMLManager::GetInstance()->LoadNotes("save/notes.xml", phone->notes);
	XMLManager::GetInstance()->LoadChats("save/chats.xml", phone->chats);

	puzzles.insert(std::pair<std::string, Puzzle*>("BookshelfPuzzle", new BookshelfPuzzle()));
	puzzles.insert(std::pair<std::string, Puzzle*>("Numpad_Backdoor", new NumpadPuzzle()));
	puzzles.insert(std::pair<std::string, Puzzle*>("Numpad_Emma", new NumpadPuzzle_2()));
	puzzles.insert(std::pair<std::string, Puzzle*>("BookshelfPuzzle2", new BookshelfPuzzle_2()));
	puzzles.insert(std::pair<std::string, Puzzle*>("PuppetPuzzle", new PuppetPuzzle()));
	PuzzleTime = false;

	pause = new OpenPauseWindowButton("Texture/UI/GameUI/Pause_button.png");
	pause->SetSize(60, -60);
	pause->SetPosition(glm::vec3(600, 300, 1));
	pause->SetCollder(new Collider(pause));

	UI.push_back(phoneIcon);
	UI.push_back(pause);

	windows.push_back(PauseWindow::GetInstance());
	windows.push_back(ViewWindow::GetInstance());
	windows.push_back(SaveLoadWindow::GetInstance());
	windows.push_back(SettingWindow::GetInstance());

	cutscene = new UIObject();
	cutscene->SetSize(1280, 720);

	butler->Appear();
	player->SetPosition(glm::vec3(currentLevel->xStart, currentLevel->GetCurrentRoom()->y, 1));
}

Item* GameScreen::FindItem(std::string name) {
	//std::cout << items.size();
	for (auto i : items)
		if (i->name == name)
			return i;
	//std::cout << "cannot find " << name << std::endl;
	return nullptr;
}

Puzzle* GameScreen::FindPuzzle(std::string name)
{
	if(!name.empty())
		return puzzles[name];
	return nullptr;
}

void GameScreen::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();

	if (PuzzleTime)
		currentPuzzle->Render();
	else {
		currentLevel->Render();
		renderer->Render(UI);
	}
	inventory->Render();

	if (phone->open)
		phone->Render();
	if (dialogueText->IsDisplay())
		dialogueText->Render();

	if (GameWindowOpen())
		for (auto w : windows)
			w->Render();
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
	}
	//if (InventoryEnable && !phone->open)
	inventory->Update();
}

void GameScreen::RightClick(glm::vec3 screen, glm::vec3 world) {
	if (PuzzleTime) {
		currentPuzzle->RightClick(screen, world);
	}
	else if (dialogueText->IsDisplay() && !Pause)
		dialogueText->clickLeft(screen);
	else if (!PuzzleTime) {
		inventory->UnselectItem();
		if (!phone->open && !player->anim->IsPlaying("Pickup") && !GameWindowOpen() && !dialogueText->IsDisplay())
			currentLevel->RightClick(world.x, world.y);
	}
	
}

void GameScreen::LeftClick(glm::vec3 screen, glm::vec3 world) {
	if (Pause) {
		PauseWindow::GetInstance()->LeftClick(screen.x, screen.y);
	}
	else {
		buttonClicked = false;
		for (int j = 0; j < UI.size(); j++) {
			if (Button * button = dynamic_cast<Button*>(UI[j])) {
				button->checkColliderPressed(screen.x, screen.y);
			}
		}
		if (GameWindowOpen() || dialogueText->IsDisplay() && !buttonClicked) {
			if (GameWindowOpen())
				for (auto w : windows)
					w->LeftClick(screen.x, screen.y);
			if (dialogueText->IsDisplay() && !buttonClicked)
				dialogueText->clickLeft(screen);
		}
		else {
			if (phone->open)
				phone->LeftClick(screen.x, screen.y);
			else if (PuzzleTime)
				currentPuzzle->LeftClick(screen, world);
			else {
				if (InventoryEnable && !buttonClicked) {
					inventory->LeftClick(screen.x, screen.y);
				}
				if (!buttonClicked && !player->anim->IsPlaying("Pickup") && !PuzzleTime)
					currentLevel->LeftClick(world.x, world.y);
			}
		}
	}
}

void GameScreen::RightRelease(glm::vec3 screen, glm::vec3 world)
{
	if (PuzzleTime) {
		currentPuzzle->RightRelease(screen, world);
	}
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
	else if (InventoryEnable)
		inventory->LeftRelease(screen.x, screen.y);
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
	player->SetPosition(glm::vec3(currentLevel->xStart, currentLevel->GetCurrentRoom()->y, 1));
	player->StopWalking();
	player->SetWalkLimit(currentLevel->GetCurrentRoom()->GetPlayerWalkLimit());
	Camera* camera = Game::GetInstance()->GetCamera();
	camera->SetLimit(currentLevel->GetCurrentRoom()->GetCameraLimit());
	dialogueText->setText(currentLevel->GetCurrentRoom()->dialogue);
	currentLevel->GetCurrentRoom()->dialogue.clear();
	switch (level)
	{
	case 0:
		SoundManager::GetInstance()->stopAllSounds();
		//SoundManager::GetInstance()->playSound(BGM, "Mainfloor1");
		// floor 1
		break;
	case 1:
		// floor 2
		SoundManager::GetInstance()->stopAllSounds();
		SoundManager::GetInstance()->playSound(BGM, "Mainfloor2");
		break;
	case 2:
		// floor 3
		SoundManager::GetInstance()->stopAllSounds();
		SoundManager::GetInstance()->playSound(BGM, "Mainfloor3");
		// assuming the Butler::PHASE is already set using the dialogue
		butler->Appear();
		dialogueText->setText(currentLevel->GetCurrentRoom()->dialogue);
		break;
	default:
		break;
	}
}

void GameScreen::ChangeRoom(std::string room, std::string door) {
	if(!room.empty() && !door.empty())
		currentLevel->ChangeRoom(room, door);
}

InteractTypeList GameScreen::GetPointedObject(glm::vec3 pos) {
	std::vector<DrawableObject*> objects = currentLevel->GetCurrentRoom()->objects;
	for (int i = (int)objects.size() - 1; i >= 0; i--)
	{
		if (InteractableObj* obj = dynamic_cast<InteractableObj*>(objects[i]))
			if(obj->CheckPointing(pos.x, pos.y))
			{
				//std::cout << obj->object_name << ", " << obj->getType() <<  std::endl;
				return obj->getType();
			}
	}
	std::vector<DrawableObject*> npcs = currentLevel->GetCurrentRoom()->npcs;
	for (int i = (int)npcs.size() - 1; i >= 0; i--)
	{
		if (InteractableObj* npc = dynamic_cast<InteractableObj*>(npcs[i]))
			if(npc->CheckPointing(pos.x, pos.y))
			{
				//std::cout << obj->object_name << ", " << obj->getType() <<  std::endl;
				return npc->getType();
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
	Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_PUZZLE_ON, true);
}

void GameScreen::ResetPuzzle() {
	for (auto puzzle : puzzles)
		puzzle.second->Reset();
}

void GameScreen::ClosePuzzle() {
	PuzzleTime = false;
	InventoryEnable = true;
	Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_PUZZLE_ON, false);
}

void GameScreen::HandleKey(SDL_Keycode key) {
	/*switch (key)
	{
	case SDLK_1:
		puzzles["BookshelfPuzzle"]->CompletePuzzle();
		break;
	case SDLK_2:
		puzzles["Numpad_Emma"]->CompletePuzzle();
		break;
	case SDLK_3:
		puzzles["Numpad_Backdoor"]->CompletePuzzle();
		((NumpadPuzzleAfter*)currentLevel->rooms["BackDoorRoomBasement"]->FindObject("NumpadAfterUnlock"))->UnlockBookshelf();
		break;
	case SDLK_4:
		puzzles["BookshelfPuzzle2"]->CompletePuzzle();
		break;
	case SDLK_q:
		Game::GetInstance()->TriggerChangeLevel(1);
		break;
	case SDLK_s:
		player->walkSpeed = 1000;
		break;
	case SDLK_a:
		player->TriggerRouteA();
		break;
	case SDLK_d:
		player->TriggerRouteB();
		break;
	default:
		break;
	}*/
}

GameScreen::~GameScreen() {
	delete player;
	delete butler;
	delete phone;
	delete phoneIcon;
	delete pause;
	delete currentLevel;
	delete dialogueText;

	for (auto p : puzzles)
		delete p.second;

	for (auto ui : UI)
		delete ui;

	for (auto item : items)
		delete item;

	for (auto p : puzzles)
		delete p.second;

	delete inventory;
}

/*CUTSCENE*/
CreditScreen::CreditScreen() {
	TextObject* credit = new TextObject();
	std::string creditText;
	float startY = -380;
	float startX = -450;


}

void CreditScreen::Render() {
	// play the cutscene
}
void CreditScreen::Update() {
	// eg. 2 clicks to skip a cutscene
}

void CreditScreen::LeftClick(glm::vec3, glm::vec3) {

}

void CreditScreen::RightClick(glm::vec3, glm::vec3)
{

}

void CreditScreen::RightRelease(glm::vec3, glm::vec3)
{

}

void CreditScreen::LeftRelease(glm::vec3, glm::vec3)
{

}

void CreditScreen::UpdateMouseState(glm::vec3, glm::vec3)
{

}

void CreditScreen::HandleKey(SDL_Keycode key) {

}

/*TestScene NumPad*/

TestScreen::TestScreen()
{
	puzzle = new Bookshelf2("Texture/puzzle5/Puzzle5_BookShelf.png", 0, 0, 1280, -720);
	//void Init(std::vector<Book2*>, std::vector<Space2*>, std::vector<UIObject*>, std::vector<Paper*>);
	std::vector<Book2*> books;
	std::vector<Book*> books_1;
	std::vector<Space*> spaces;
	std::vector<UIObject*> images;
	std::vector<Paper*> papers;

	Book2* tmp_book = new Book2(11, "Texture/puzzle5/Puzzle5_IncomBook1.png", 65, -155, 0, 268, 1);
	books.push_back(tmp_book);
	tmp_book = new Book2(12, "Texture/puzzle5/Puzzle5_IncomBook2.png", 65, -155, 0, 98, 5);
	books.push_back(tmp_book);
	tmp_book = new Book2(13, "Texture/puzzle5/Puzzle5_IncomBook3.png", 65, -155, 0, -89, 6);
	books.push_back(tmp_book);
	tmp_book = new Book2(14, "Texture/puzzle5/Puzzle5_IncomBook4.png", 65, -155, 0, -270, 4);
	books.push_back(tmp_book);


	Book* tmp_book1 = new Book(4, "Texture/puzzle5/Puzzle5_WhiteBook4.png", 65, -155, -107, 268);
	books_1.push_back(tmp_book1);
	Space* tmp_space = new Space(1, glm::vec3(-107, 268, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(8, "Texture/puzzle5/Puzzle5_WhiteBook8.png", 65, -155, 107, 268);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(2, glm::vec3(107, 268, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(3, "Texture/puzzle5/Puzzle5_WhiteBook3.png", 65, -155, -107, 93);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(3, glm::vec3(-107, 93, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(1, "Texture/puzzle5/Puzzle5_WhiteBook1.png", 65, -155, 107, 93);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(4, glm::vec3(107, 93, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(6, "Texture/puzzle5/Puzzle5_WhiteBook6.png", 65, -155, -107, -88);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(5, glm::vec3(-107, -88, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(5, "Texture/puzzle5/Puzzle5_WhiteBook5.png", 65, -155, 107, -88);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(6, glm::vec3(107, -88, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(7, "Texture/puzzle5/Puzzle5_WhiteBook7.png", 65, -155, -107, -269);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(7, glm::vec3(-107, -269, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(2, "Texture/puzzle5/Puzzle5_WhiteBook2.png", 65, -155, 107, -269);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(8, glm::vec3(107, -269, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	UIObject* tmp_image = new UIObject();
	tmp_image->SetTexture("Texture/puzzle5/Puzzle5_BookShelf.png");
	tmp_image->SetSize(1280, -720);
	tmp_image->SetPosition(glm::vec3(0, 0, 0));
	images.push_back(tmp_image);

	Paper* tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper1(I).png", "Texture/puzzle5/Puzzle5_Paper1.png", glm::vec3(338, 263, 0), 229, -161, 1);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper2(I).png", "Texture/puzzle5/Puzzle5_Paper2.png", glm::vec3(338, 68, 0), 229, -161, 2);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper3(I).png", "Texture/puzzle5/Puzzle5_Paper3.png", glm::vec3(338, -89, 0), 229, -161, 3);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper4(I).png", "Texture/puzzle5/Puzzle5_Paper4.png", glm::vec3(338, -247, 0), 229, -161, 4);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper5(I).png", "Texture/puzzle5/Puzzle5_Paper5.png", glm::vec3(497, 263, 0), 229, -161, 5);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper6(I).png", "Texture/puzzle5/Puzzle5_Paper6.png", glm::vec3(497, 68, 0), 229, -161, 6);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper7(I).png", "Texture/puzzle5/Puzzle5_Paper7.png", glm::vec3(497, -89, 0), 229, -161, 7);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper8(I).png", "Texture/puzzle5/Puzzle5_Paper8.png", glm::vec3(497, -247, 0), 229, -161, 8);
	papers.push_back(tmp_paper);

	((Bookshelf2*)puzzle)->Init(books_1, books, spaces, images, papers);
}

void TestScreen::Init()
{

}

void TestScreen::Render()
{
	((Bookshelf2*)puzzle)->Render();
}

void TestScreen::Update()
{
	((Bookshelf2*)puzzle)->Update();
}

void TestScreen::RightClick(glm::vec3 screen, glm::vec3 world)
{
	((Bookshelf2*)puzzle)->RightClick(screen, world);
}

void TestScreen::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	((Bookshelf2*)puzzle)->LeftClick(screen, world);
}

void TestScreen::UpdateMouseState(glm::vec3 screen, glm::vec3 world)
{

}

TestScreen::~TestScreen()
{

}

void TestScreen::RightRelease(glm::vec3 screen, glm::vec3 world)
{
	((Bookshelf2*)puzzle)->RightRelease(screen, world);
}

void TestScreen::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	((Bookshelf2*)puzzle)->LeftRelease(screen, world);
}

void TestScreen::HandleKey(SDL_Keycode)
{

}