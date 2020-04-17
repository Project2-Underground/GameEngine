#include "GameWindows.h"
#include "Game.h"

ViewWindow* ViewWindow::instance = nullptr;

ViewWindow* ViewWindow::GetInstance() {
	if (instance == nullptr) {
		instance = new ViewWindow();
		Game* g = Game::GetInstance();
		instance->Init(g->winWidth, g->winHeight);
	}
	return instance;
}
ViewWindow::ViewWindow() {
	closeButton = new ViewWindowCloseButton("Texture/Puzzle/CloseButton.png");
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
	bgWindow->Init((float)width * 0.5f, -(float)height * 0.5f, glm::vec3(0, (float)height * 0.5f * 0.3f, 0.0f));
	viewItem->SetSize(540, -540);
	viewItem->SetPosition(bgWindow->getPos());
	float closeButtonSize = 50.0f;
	closeButton->Init(closeButtonSize, -closeButtonSize,
					  glm::vec3(bgWindow->col->getMaxBound().x - closeButtonSize * 0.75f,
					  bgWindow->col->getMaxBound().y - closeButtonSize * 0.75f,
					  0.0f));
}

void ViewWindow::SetViewItem(Item* item) {
	viewItem->SetTexture(item->GetViewTexture());
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
	if (display) {
		closeButton->checkColliderPressed(x, y);
	}
}

void ViewWindow::LeftRelease(float x, float y) {
	if (display) {
		closeButton->checkColliderReleased(x, y);
	}
}

void ViewWindow::UpdateMouseButton(glm::vec3 screen) {
	if (display) {
		closeButton->updateButton(screen.x, screen.y);
	}
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
