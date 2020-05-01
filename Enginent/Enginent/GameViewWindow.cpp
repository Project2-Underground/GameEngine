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

	//bgWindow->SetDisplay(false);
	bgWindow->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");
}

void GameWindow::Update() {
	Game* g = Game::GetInstance();
	if (trigger) {
		bool playerNotPicking = g->GetPlayer() && g->GetPlayer()->anim && !g->GetPlayer()->anim->IsPlaying("Pickup");
		if (g->GetScreenState() == MENUSCREEN || playerNotPicking) {
			display = true;
			trigger = false;
		}
	}
	if (display) {
		CursorUI* cursor = Game::GetInstance()->GetCursor();
		cursor->enableChange(false);
	}
}

void ViewWindow::Init(int width, int height) {
	//set size, pos of this, description box and viewItem
	bgWindow->Init((float)width * 0.45f, -(float)height * 0.5f, glm::vec3(0, (float)height * 0.5f * 0.3f, 0.0f));
	viewItem->SetSize(540, -540);
	viewItem->SetPosition(bgWindow->getPos());
	float closeButtonSize = 40.0f;
	closeButton->Init(closeButtonSize, -closeButtonSize,
					  glm::vec3(bgWindow->col->getMaxBound().x + closeButtonSize * 0.5f,
					  bgWindow->col->getMaxBound().y - closeButtonSize * 0.75f,
					  0.0f));
}

void ViewWindow::SetViewItem(Item* item) {
	float sizeX = item->width;
	float sizeY = item->height;
	if (-item->height > 500) {
		sizeX = 500 * item->aspect;
		sizeY = -500;
	}
	viewItem->SetSize(sizeX, sizeY);
	viewItem->SetTexture(item->GetViewTexture());
	if (viewItem->getSize().x > bgWindow->getSize().x) {
		closeButton->SetPosition(glm::vec3(viewItem->getSize().x * 0.5f + closeButton->getSize().x * 0.5f, closeButton->getPos().y, 0));
	}
}

void ViewWindow::SetViewItem(ViewObj* obj) {
	float sizeX = obj->width;
	float sizeY = obj->height;
	if (-obj->height > 500) {
		float aspect = obj->width / -obj->height;
		sizeX = 500 * aspect;
		sizeY = -500;
	}
	viewItem->SetSize(sizeX, sizeY);
	viewItem->SetTexture(obj->GetViewTexture());
	if (viewItem->getSize().x > bgWindow->getSize().x) {
		closeButton->SetPosition(glm::vec3(viewItem->getSize().x * 0.5f + closeButton->getSize().x * 0.5f, closeButton->getPos().y, 0));
	}
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
