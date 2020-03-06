#include "GameWindows.h"
#include "Game.h"

SettingWindow* SettingWindow::instance = nullptr;

SettingWindow* SettingWindow::GetInstance() {
	if (!instance)
		instance = new SettingWindow();
	return instance;
}

void SettingWindow::Update() {

}
void SettingWindow::Init(int width, int height) {

}
void SettingWindow::Render() {

}
void SettingWindow::LeftClick(float, float) {

}
void SettingWindow::LeftRelease(float, float) {

}
void SettingWindow::UpdateMouseButton(glm::vec3) {

}
SettingWindow::~SettingWindow() {

}