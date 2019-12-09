#include "Screen.h"
#include "Game.h"

/*MAIN MENU*/

MenuScreen::MenuScreen() {
	std::cout << "hi\n";
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

	UIs.push_back(background);
	UIs.push_back(play);
	UIs.push_back(quit);
}

void MenuScreen::Render() {
	Game::GetInstance()->GetRenderer()->Render(UIs);
}

void MenuScreen::Update() {

}

void MenuScreen::LeftClick(int x, int y) {
	glm::vec3 tmp = Game::GetInstance()->FindMousePosition(x, y);
	for (int j = 0; j < UIs.size(); j++)
	{
		if (Button* button = dynamic_cast<Button*>(UIs[j]))
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
	player->SetWalkLimit(currentLevel->GetCurrentRoom()->GetRoomLimit());

	UIObject* blackBold1 = new UIObject();
	blackBold1->SetTexture("Texture/UI/Black_Border.png");
	blackBold1->SetSize(1280, -720);
	blackBold1->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	UI.push_back(blackBold1);
}

void GameScreen::Render() {
	currentLevel->Render();
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(player);
	renderer->Render(UI);
}

void GameScreen::Update() {
	currentLevel->Update();
	player->Update();
}

void GameScreen::RightClick(int x, int y) {
	currentLevel->RightClick(x, y);
}

void GameScreen::LeftClick(int x, int y) {
	currentLevel->LeftClick(x, y);
}

void GameScreen::ChangeLevel(int level) {
	// save current level to xml
	delete currentLevel;
	currentLevel = new Level(levels[level]);
}

void GameScreen::AddUI(UIObject* uio) {
	//UI.push_back(uio);
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