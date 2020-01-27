#include "GameViewWindow.h"

ViewWindow* ViewWindow::instance = nullptr;

ViewWindow* ViewWindow::GetInstance() {
	if (instance == nullptr)
		instance = new ViewWindow();
	return instance;
}
ViewWindow::ViewWindow() {
	display = false;
	closeButton = new ViewWindowClose("texture");
	//set size, pos of this, description box and viewItem
}

void ViewWindow::SetViewItem(unsigned int texture) {
	viewItem->SetTexture(texture);
}

void ViewWindow::SetText() {

}