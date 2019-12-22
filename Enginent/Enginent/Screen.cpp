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

	quit = new Exit_Button("Texture/UI/MainScreen/ExitBotton_Normal.png", "Texture/UI/MainScreen/ExitBotton_Point.png", "Texture/UI/MainScreen/ExitBotton_Click.png");;
	quit->SetSize(300, -120);
	quit->SetPosition(glm::vec3(-170.0f, -50.0f, 1.0f));
	col = new Collider(quit);
	quit->SetCollder(col);

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

void MenuScreen::LeftClick(int x, int y) {
	glm::vec3 tmp = Game::GetInstance()->FindMousePosition(x, y);
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button* button = dynamic_cast<Button*>(UI[j]))
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

/*GAME SCREEN*/
GameScreen::GameScreen(int level) {
	levels.push_back("save/level1.xml");

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

	UIObject* blackBold1 = new UIObject();
	blackBold1->SetTexture("Texture/UI/Black_Border.png");
	blackBold1->SetSize(1280, -720);
	blackBold1->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	UI.push_back(blackBold1);

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
		/*glm::vec3 tmp = Game::GetInstance()->FindMousePosition(x, y);
		phone->ClickButton(tmp.x, tmp.y);*/
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

/*CUTSCENE*/
CutsceneScreen::CutsceneScreen() {

}

void CutsceneScreen::Render() {
	// play the cutscene
}

void CutsceneScreen::Update() {
	// 2 clicks to skip cutscene
}

void CutsceneScreen::LeftClick(int x, int y) {

}