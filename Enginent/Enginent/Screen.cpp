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
	// filepath of levels
	levels.push_back("save/level1.xml");
	//levels.push_back("save/level2.xml");
	//levels.push_back("save/level3.xml");

	currentLevel = new Level(levels[0]);
	player = new Player();
	player->SetTexture("Texture/Character/Elias_idle.png");
	player->SetSize(337.5f, -834.0f);
	player->SetPosition(glm::vec3(0.0f, -50.0f, 1.0f));
	player->SetCollder(new Collider(player));
	player->SetWalkLimit(currentLevel->GetCurrentRoom()->GetPlayerWalkLimit());
	player->createDialogueText();

	Camera* camera = Game::GetInstance()->GetCamera();
	camera->SetTarget(player);
	camera->SetLimit(currentLevel->GetCurrentRoom()->GetCameraLimit());

	Game* g = Game::GetInstance();
	viewWin = ViewWindow::GetInstance();
	viewWin->Init(g->winWidth, g->winHeight);

	inventory = new Inventory();
	phone = Phone::GetInstance();
}

void GameScreen::LoadGame(std::string filename) {
	XMLManager::GetInstance()->LoadFromSave(filename);
}

void GameScreen::Render() {
	currentLevel->Render();
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(player);
	renderer->Render(player->dialogueText);
	renderer->Render(currentLevel->GetCurrentRoom()->foreground, false);
	renderer->Render(UI);
	inventory->Render();
	viewWin->Render();
	if (phone->open)
		phone->Render();
}

void GameScreen::Update() {
	currentLevel->Update();
	player->Update();
	inventory->Update();
}

void GameScreen::RightClick(glm::vec3 screen, glm::vec3 world) {
	inventory->UnselectItem();
	if (!phone->open)
		currentLevel->RightClick(world.x, world.y);
}

void GameScreen::LeftClick(glm::vec3 screen, glm::vec3 world) {
	if (phone->open) {
		phone->LeftClick(screen.x, screen.y);
	}
	else if (viewWin->IsOpen()) {
		viewWin->LeftClick(screen.x, screen.y);
	}
	else {
		currentLevel->LeftClick(world.x, world.y);
	}
	inventory->LeftClick(screen.x, screen.y);
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

void GameScreen::HandleKey(SDL_Keycode key) {
	switch (key)
	{
	case SDLK_d: {
		// unlock door
		Item* item = inventory->GetInventoryBox(0)->GetItem();
		if (item != nullptr)
			currentLevel->GetCurrentRoom()->doors["EliasRoomInnerDoor"]->UseItem(item);
	}break;
	case SDLK_w: {
		Item* item = inventory->GetInventoryBox(0)->GetItem();
		if (item != nullptr && dynamic_cast<SeparatableItem*>(item)) 
			((SeparatableItem*)item)->action();
	}break;
	case SDLK_s:
		// save current game
		XMLManager::GetInstance()->SaveGame("save/test.xml");
		break;
	case SDLK_l:
		XMLManager::GetInstance()->LoadFromSave("save/test.xml");
		break;
	case SDLK_p:
		Phone::GetInstance()->open = !Phone::GetInstance()->open;
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

TestScreen::TestScreen()
{
	std::vector<Book*> books1;
	std::vector<Space*> space1;
	std::vector<ImageObject* > images;

	background = new UIObject();
	background->SetTexture("Texture/Puzzle/Puzzle1_Background.png");
	background->SetSize(1280, -720);
	UI.push_back(background);

	ImageObject* bg = new ImageObject();
	bg->SetTexture("Texture/Puzzle/Puzzle1_Background.png");
	bg->SetSize(1280, -720);
	images.push_back(bg);

	ImageObject* shelf = new ImageObject();
	shelf->SetTexture("Texture/Puzzle/Puzzle1_BookShelf.png");
	shelf->SetPosition(glm::vec3(-213, 0, 1));
	shelf->SetSize(455, -736);
	images.push_back(shelf);

	ImageObject* block = new ImageObject();
	block->SetTexture("Texture/Puzzle/Puzzle1_Boxes.png");
	block->SetPosition(glm::vec3(358, 8, 1));
	block->SetSize(358, -417);
	images.push_back(block);

	Book* book1 = new Book(1, "Texture/Puzzle/Puzzle1_Book1.png", 56, -154, 252, 87);
	Book* book2 = new Book(2, "Texture/Puzzle/Puzzle1_Book2.png", 56, -154, 352, 87);
	Book* book3 = new Book(3, "Texture/Puzzle/Puzzle1_Book3.png", 56, -154, 448, 87);
	Book* book4 = new Book(4, "Texture/Puzzle/Puzzle1_Book4.png", 56, -154, 252, -88);
	Book* book5 = new Book(5, "Texture/Puzzle/Puzzle1_Book5.png", 56, -154, 448, -88);
	Book* book6 = new Book(6, "Texture/Puzzle/Puzzle1_Book6.png", 56, -154, 352, -88);
	Space* s1 = new Space(1, glm::vec3(-185, 93, 0), 57, -155);
	Space* s2 = new Space(2, glm::vec3(-303, 93, 0), 57, -155);
	Space* s3 = new Space(3, glm::vec3(-243, 267, 0), 57, -155);
	Space* s4 = new Space(4, glm::vec3(-242, -85, 0), 57, -155);
	Space* s5 = new Space(5, glm::vec3(-124, -85, 0), 57, -155);
	Space* s6 = new Space(6, glm::vec3(-303, -266, 0), 57, -155);

	books1.push_back(book1);
	books1.push_back(book2);
	books1.push_back(book3);
	books1.push_back(book4);
	books1.push_back(book5);
	books1.push_back(book6);
	space1.push_back(s3);
	space1.push_back(s2);
	space1.push_back(s4);
	space1.push_back(s1);
	space1.push_back(s5);
	space1.push_back(s6);

	puzzle = new BookshelfPuzzle("Texture/Puzzle/Puzzle1_BookShelf.png", -300, 0, 200, -400);
	((BookshelfPuzzle* )puzzle)->Init(books1, space1, images);
	UI.push_back(puzzle);

}

void TestScreen::Render()
{
	Game::GetInstance()->GetRenderer()->Render(UI);
}

void TestScreen::Update()
{

}

void TestScreen::RightClick(glm::vec3 screen, glm::vec3 world)
{

}

void TestScreen::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	if (puzzle->IsDisplay()) {
		puzzle->LeftClick(screen, world);
	}
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkCollider(screen.x, screen.y);
		}
	}
}

void TestScreen::HandleKey(SDL_Keycode)
{

}

void TestScreen::UpdateMouseState(glm::vec3 screen, glm::vec3 world)
{
	if(puzzle->IsDisplay())
	((BookshelfPuzzle*)puzzle)->Update(screen, world);
}

TestScreen::~TestScreen()
{
	for (auto ui : UI)
		delete ui;
}