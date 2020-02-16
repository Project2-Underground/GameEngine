#include "GameViewWindow.h"
#include "Game.h"

ViewWindow* ViewWindow::instance = nullptr;

ViewWindow* ViewWindow::GetInstance() {
	if (instance == nullptr)
		instance = new ViewWindow();
	return instance;
}
ViewWindow::ViewWindow() {
	closeButton = new ViewWindowClose("Texture/tmp_closeButton.png");
	viewItem = new UIObject();
	viewWindow = new UIObject();
	display = false;

	viewWindow->SetTexture("Texture/tmp_inventoryBox.png");
}

void ViewWindow::Init(int width, int height) {
	//set size, pos of this, description box and viewItem
	viewWindow->SetSize((float)width * 0.5f, -(float)height * 0.5f);
	viewItem->SetSize(540, -540);
	viewItem->SetPosition(glm::vec3(viewWindow->getPos().x, viewWindow->getPos().y + -viewItem->getSize().y * 0.2f, 0.0f));
	closeButton->SetSize(viewWindow->getSize().x * 0.5f, viewWindow->getSize().y * 0.1f);
	closeButton->SetPosition(glm::vec3(viewWindow->getPos().x, viewWindow->getPos().y - -viewWindow->getSize().y * 0.5 + -closeButton->getSize().y, 0.0f));
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
	delete viewWindow;
}

void ViewWindow::Render() {
	if (display) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();
		renderer->Render(viewWindow);
		renderer->Render(viewItem);
		renderer->Render(closeButton);
	}
}

void ViewWindow::LeftClick(float x, float y) {
	closeButton->checkCollider(x, y);
}

void ViewWindow::Close() {
	display = false;
}

void ViewWindow::Open() {
	display = true;
}