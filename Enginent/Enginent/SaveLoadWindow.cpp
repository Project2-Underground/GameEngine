#include "GameWindows.h"
#include "Game.h"

SaveLoadWindow::SaveLoadWindow() {
	closeButton = new SaveLoadWindowCloseButton("Texture/tmp_texture/tmp_closeButton.png");
	bgWindow = new UIObject();
	bgWindow->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");
}

SaveLoadWindow::~SaveLoadWindow() {
	delete closeButton;
	delete bgWindow;
}

void SaveLoadWindow::Init(int width, int height) {
	bgWindow->SetSize((float)width * 0.5f, -(float)height * 0.75f);

	SaveLoadGameButton* tmp1 = new SaveLoadGameButton("Texture/tmp_texture/tmp_savefile.png", "save/save1.xml");
	tmp1->SetPressTexture("Texture/tmp_texture/tmp_savefilePress.png");
	tmp1->SetHoverTexture("Texture/tmp_texture/tmp_savefileHover.png");
	tmp1->Init(320.0f, -90.0f,glm::vec3(0,200,1));
	tmp1->InitSaveLoadButton();

	SaveLoadGameButton* tmp2 = new SaveLoadGameButton("Texture/tmp_texture/tmp_savefile.png", "save/save2.xml");
	tmp2->SetPressTexture("Texture/tmp_texture/tmp_savefilePress.png");
	tmp2->SetHoverTexture("Texture/tmp_texture/tmp_savefileHover.png");
	tmp2->Init(320.0f, -90.0f,glm::vec3(0,90,1));
	tmp2->InitSaveLoadButton();

	SaveLoadGameButton* tmp3 = new SaveLoadGameButton("Texture/tmp_texture/tmp_savefile.png", "save/save3.xml");
	tmp3->SetPressTexture("Texture/tmp_texture/tmp_savefilePress.png");
	tmp3->SetHoverTexture("Texture/tmp_texture/tmp_savefileHover.png");
	tmp3->Init(320.0f, -90.0f,glm::vec3(0,-20,1));
	tmp3->InitSaveLoadButton();

	SaveLoadGameButton* tmp4 = new SaveLoadGameButton("Texture/tmp_texture/tmp_savefile.png", "save/save4.xml");
	tmp4->SetPressTexture("Texture/tmp_texture/tmp_savefilePress.png");
	tmp4->SetHoverTexture("Texture/tmp_texture/tmp_savefileHover.png");
	tmp4->Init(320.0f, -90.0f,glm::vec3(0,-130,1));
	tmp4->InitSaveLoadButton();

	saveButtons.push_back(tmp1);
	saveButtons.push_back(tmp2);
	saveButtons.push_back(tmp3);
	saveButtons.push_back(tmp4);

	closeButton->SetSize(bgWindow->getSize().x * 0.5f, bgWindow->getSize().y * 0.1f);
	closeButton->SetPosition(glm::vec3(bgWindow->getPos().x, bgWindow->getPos().y - -bgWindow->getSize().y * 0.5 + -closeButton->getSize().y, 0.0f));
	closeButton->SetCollder(new Collider(closeButton));
}

void SaveLoadWindow::Render() {
	if (display) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();
		renderer->Render(bgWindow);
		for (auto s : saveButtons) {
			renderer->Render(s);
			renderer->Render(s->name);
			renderer->Render(s->details);
		}
		//renderer->Render(save);
		renderer->Render(closeButton);
	}
}

SaveLoadWindow* SaveLoadWindow::instance = nullptr;

SaveLoadWindow* SaveLoadWindow::GetInstance() {
	if (instance == nullptr) {
		instance = new SaveLoadWindow();
		Game* g = Game::GetInstance();
		instance->Init(g->winWidth, g->winHeight);
	}
	return instance;
}

void SaveLoadWindow::LeftClick(float x, float y) {
	if (display) {
		closeButton->checkColliderPressed(x, y);
		for (auto s : saveButtons)
			s->checkColliderPressed(x, y);
		//save->checkColliderPressed(x, y);
	}
}

void SaveLoadWindow::LeftRelease(float x, float y) {
	if (display) {
		closeButton->checkColliderReleased(x, y);
		for (auto s : saveButtons)
			s->checkColliderReleased(x, y);
		//save->checkColliderReleased(x, y);
	}
}

void SaveLoadWindow::Update() {
	if (trigger) {
		display = true;
		trigger = false;
	}	
}

void SaveLoadWindow::UpdateMouseButton(glm::vec3 screen) {
	if (display) {
		closeButton->updateButton(screen.x, screen.y);
		for (auto s : saveButtons)
			s->updateButton(screen.x, screen.y);
		//save->updateButton(screen.x, screen.y);
	}
}