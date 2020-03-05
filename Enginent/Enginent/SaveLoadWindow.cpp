#include "GameWindows.h"
#include "Game.h"

SaveLoadWindow::SaveLoadWindow() {
	closeButton = new WindowClose("Texture/tmp_texture/tmp_closeButton.png");
	bgWindow = new UIObject();
	bgWindow->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");
}

SaveLoadWindow::~SaveLoadWindow() {
	delete save;
	delete closeButton;
	delete bgWindow;
}

void SaveLoadWindow::Init(int width, int height) {
	bgWindow->SetSize((float)width * 0.5f, -(float)height * 0.5f);

	save = new SaveLoadGameButton("Texture/tmp_texture/tmp_savefile.png", "save/save1.xml");
	save->SetPressTexture("Texture/tmp_texture/tmp_savefilePress.png");
	save->SetHoverTexture("Texture/tmp_texture/tmp_savefileHover.png");
	save->SetSize((float)width * 0.25f, -(float)height * 0.125f);
	save->SetCollder(new Collider(save));

	closeButton->SetSize(bgWindow->getSize().x * 0.5f, bgWindow->getSize().y * 0.1f);
	closeButton->SetPosition(glm::vec3(bgWindow->getPos().x, bgWindow->getPos().y - -bgWindow->getSize().y * 0.5 + -closeButton->getSize().y, 0.0f));
	closeButton->SetCollder(new Collider(closeButton));
}

void SaveLoadWindow::Render() {
	if (display) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();
		renderer->Render(bgWindow);
		renderer->Render(save);
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
	closeButton->checkColliderPressed(x, y);
	save->checkColliderPressed(x, y);
}

void SaveLoadWindow::LeftRelease(float x, float y) {
	closeButton->checkColliderReleased(x, y);
	save->checkColliderReleased(x, y);
}

void SaveLoadWindow::Update() {
	if (trigger) {
		display = true;
		trigger = false;
	}	
}

void SaveLoadWindow::UpdateMouseButton(glm::vec3 screen) {
	closeButton->updateButton(screen.x, screen.y);
	save->updateButton(screen.x, screen.y);
}