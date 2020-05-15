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
	viewItem = new UIObject();
	bgWindow = new UIObject();
	display = false;
	trigger = false;
	itemSizeY = 450.0f;

	bgWindow->SetTexture("Texture/tmp_texture/tmp_pausebg.png");
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
	}
}

void ViewWindow::Init(int width, int height) {
	//set size, pos of this, description box and viewItem
	bgWindow->Init(200,-200, glm::vec3(0, (float)height * 0.5f * 0.3f, 0.0f));
	viewItem->SetSize(540, -540);
	viewItem->SetPosition(bgWindow->getPos());
}

void ViewWindow::SetViewItem(Item* item) {
	playItemSound = true;
	float sizeX = item->width;
	float sizeY = item->height;
	if (abs(sizeY) > itemSizeY) {
		sizeX = itemSizeY * item->aspect;
		sizeY = -itemSizeY;
	}
	//std::cout << sizeX << " " << sizeY << std::endl;
	viewItem->SetSize(sizeX, sizeY);
	viewItem->SetTexture(item->GetViewTexture());
}

void ViewWindow::SetViewItem(ViewObj* obj) {
	float sizeX = obj->viewWidth;
	float sizeY = obj->viewHeight;
	if (abs(sizeY) > itemSizeY) {
		float aspect = sizeX / sizeY;
		sizeX = itemSizeY * aspect;
		sizeY = -itemSizeY;
	}
	viewItem->SetSize(sizeX, sizeY);
	viewItem->SetTexture(obj->GetViewTexture());
}

void ViewWindow::SetText() {

}

ViewWindow::~ViewWindow() {
	delete viewItem;
	delete bgWindow;
}

void ViewWindow::Render() {
	if (display) {
		if (playItemSound) {
			SoundManager::GetInstance()->playSound(SFX, "Key");
			playItemSound = false;
		}
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();
		//renderer->Render(bgWindow);
		renderer->Render(viewItem);
	}
}

void ViewWindow::LeftClick(float x, float y) {
	if (display) {
		Close();
	}
}

void ViewWindow::LeftRelease(float x, float y) {
}

void ViewWindow::UpdateMouseButton(glm::vec3 screen) {

}

void GameWindow::ForceClose() {
	display = false;
}
void GameWindow::Close() {
	//Game::GetInstance()->GetCursor()->enableChange(true);
	Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_OTHER_WINDOW_ON, false);
	display = false;
}

void GameWindow::Open() {
	CursorUI* cursor = Game::GetInstance()->GetCursor();
	//cursor->enableChange(false);
	cursor->EnableCursor(CURSOR_OTHER_WINDOW_ON, true);
	trigger = true;
}
