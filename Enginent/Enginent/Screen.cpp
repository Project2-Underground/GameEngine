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

	//play sound
	SoundManager::GetInstance()->playSound("MainScreen", true);
}

void MenuScreen::Render() {
	Game::GetInstance()->GetRenderer()->Render(UI);
}

void MenuScreen::Update() {

}

void MenuScreen::LeftClick(int x, int y) {
	glm::vec3 tmp = Game::GetInstance()->FindMousePosition(x, y);
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkCollider(tmp.x, tmp.y);
		}
	}
}

void MenuScreen::UpdateMouseState(int x, int y) {
	glm::vec3 realPos = Game::GetInstance()->FindMousePosition(x, y);

	play->updateButton(realPos.x, realPos.y);
	//setting->updateButton(realPos.x, realPos.y);
	quit->updateButton(realPos.x, realPos.y);
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
	player->SetSize(230.0f, -350.0f);
	player->SetPosition(glm::vec3(0.0f, -50.0f, 1.0f));
	player->SetCollder(new Collider(player));
	player->SetWalkLimit(currentLevel->GetCurrentRoom()->GetPlayerWalkLimit());
	player->createDialogueText();

	Camera* camera = Game::GetInstance()->GetCamera();
	camera->SetTarget(player);
	camera->SetLimit(currentLevel->GetCurrentRoom()->GetCameraLimit());

	// inventory
	const int inventoryNum = 5;
	float start_x = -550;
	float start_y = -300;
	float space = 20;
	int size = 100;

	glm::vec3 inventoryBoxPos[inventoryNum] = { glm::vec3(start_x,start_y,1), glm::vec3(start_x + size + space,start_y,1), glm::vec3(start_x + (size + space) * 2,start_y,1), glm::vec3(start_x + (size + space) * 3  ,start_y,1), glm::vec3(start_x + (size + space) * 4  ,start_y,1) };
	player->inventory = new Inventory(inventoryNum, inventoryBoxPos, 100);

	for (int i = 0; i < inventoryNum; i++)
		UI.push_back(player->inventory->GetInventoryBox(i));
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
	renderer->Render(UI);
	if (phone->open)
		phone->Render();
}

void GameScreen::Update() {
	currentLevel->Update();
	player->Update();
}

void GameScreen::RightClick(int x, int y) {
	if (!phone->open)
		currentLevel->RightClick(x, y);
}

void GameScreen::LeftClick(int x, int y) {
	if (phone->open) {
		glm::vec3 tmp = Game::GetInstance()->FindMousePosition(x, y);
		phone->LeftClick(tmp.x, tmp.y);
	}
	else {
		currentLevel->LeftClick(x, y);
	}
}

void GameScreen::ChangeLevel(int level) {
	// save current level to xml
	delete currentLevel;
	currentLevel = new Level(levels[level]);
}

void GameScreen::ChangeRoom(std::string room, std::string door) {
	currentLevel->ChangeRoom(room, door);
}

int GameScreen::GetPointedObject(glm::vec3 pos) {
	std::vector<DrawableObject*>* objects = currentLevel->Getobjects();
	for (int i = objects->size() - 1; i >= 0; i--)
	{
		if (dynamic_cast<InteractableObj*>((*objects)[i]) && dynamic_cast<InteractableObj*>((*objects)[i])->CheckPointing(pos.x, pos.y))
		{
			return dynamic_cast<InteractableObj*>((*objects)[i])->getType();
		}
	}
}

void GameScreen::HandleKey(SDL_Keycode key) {
	switch (key)
	{
	case SDLK_d: {
		// unlock door
		Item* item = player->inventory->GetInventoryBox(1)->GetItem();
		if (item != nullptr)
			currentLevel->GetCurrentRoom()->doors["EliasRoomInnerDoor"]->Unlock(item);
	}break;
	case SDLK_w: {
		Item* item = player->inventory->GetInventoryBox(0)->GetItem();
		if (item != nullptr && dynamic_cast<SeparatableItem*>(item)) 
			((SeparatableItem*)item)->Separate();
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

void CutsceneScreen::LeftClick(int x, int y) {

}

void CutsceneScreen::HandleKey(SDL_Keycode key) {

}