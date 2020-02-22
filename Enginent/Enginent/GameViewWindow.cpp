#include "GameWindows.h"
#include "Game.h"


ViewWindow* ViewWindow::instance = nullptr;

ViewWindow* ViewWindow::GetInstance() {
	if (instance == nullptr)
		instance = new ViewWindow();
	return instance;
}
ViewWindow::ViewWindow() {
	closeButton = new WindowClose("Texture/tmp_texture/tmp_closeButton.png");
	viewItem = new UIObject();
	bgWindow = new UIObject();
	display = false;
	trigger = false;

	bgWindow->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");
}

void ViewWindow::Update() {
	if(trigger)
		if (Game::GetInstance()->GetPlayer()->anim->IsPlaying("Idle")) {
			display = true;
			trigger = false;
		}
}

void ViewWindow::Init(int width, int height) {
	//set size, pos of this, description box and viewItem
	bgWindow->SetSize((float)width * 0.5f, -(float)height * 0.5f);
	viewItem->SetSize(540, -540);
	viewItem->SetPosition(glm::vec3(bgWindow->getPos().x, bgWindow->getPos().y + -viewItem->getSize().y * 0.2f, 0.0f));
	closeButton->SetSize(bgWindow->getSize().x * 0.5f, bgWindow->getSize().y * 0.1f);
	closeButton->SetPosition(glm::vec3(bgWindow->getPos().x, bgWindow->getPos().y - -bgWindow->getSize().y * 0.5 + -closeButton->getSize().y, 0.0f));
	closeButton->SetCollder(new Collider(closeButton));
}

void ViewWindow::SetViewItem(unsigned int texture) {
	viewItem->SetTexture(texture);
}

void ViewWindow::SetText() {

}

ViewWindow::~ViewWindow() {
	delete viewItem;
	delete closeButton;
	delete bgWindow;
}

void ViewWindow::Render() {
	if (display) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();
		renderer->Render(bgWindow);
		renderer->Render(viewItem);
		renderer->Render(closeButton);
	}
}

void ViewWindow::LeftClick(float x, float y) {
	closeButton->checkCollider(x, y);
}

void ViewWindow::UpdateMouseButton(glm::vec3 screen) {
	closeButton->updateButton(screen.x, screen.y);
}

void GameWindow::Close() {
	Game::GetInstance()->GetCursor()->enableChange(true);
	display = false;
}

void GameWindow::Open() {
	CursorUI* cursor = Game::GetInstance()->GetCursor();
	cursor->enableChange(false);
	trigger = true;
}
