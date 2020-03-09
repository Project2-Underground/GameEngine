#include "GameWindows.h"
#include "Game.h"

PauseWindow* PauseWindow::instance = nullptr;

PauseWindow* PauseWindow::GetInstance() {
	if (!instance)
		instance = new PauseWindow();
	return instance;
}
PauseWindow::PauseWindow() {
	bgWindow = new UIObject();
	bgWindow->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");
	setting = new SettingButton("");
	load = new OpenLoadSaveWindow("");
	menu = new MainMenuButton("");
	closeButton = new WindowCloseButton("");
}
bool PauseWindow::OtherWindowOpens() {
	if (SettingWindow::GetInstance()->IsOpen() || SaveLoadWindow::GetInstance()->IsOpen())
		return true;
	return false;
}
void PauseWindow::Update() {
	if (trigger) {
		display = true;
		trigger = false;
	}

	if (OtherWindowOpens() && display) {
		display = false;
		otherWindowTrigger = true;
	}
	else if (!OtherWindowOpens() && otherWindowTrigger) {
		display = true;
		otherWindowTrigger = false;
	}
}
void PauseWindow::Init(int width, int height) {
	bgWindow->SetSize((float)width * 0.5f, -(float)height * 0.75f);

	setting->SetSize(width * 0.25f, -height * 0.075f);
	setting->SetPosition(glm::vec3(0, height * 0.25f, 0));
	setting->SetCollder(new Collider(setting));

	load->SetSize(width * 0.25f, -height * 0.075f);
	load->SetPosition(glm::vec3(0, height * 0.125f, 0));
	load->SetCollder(new Collider(load));

	menu->SetSize(width * 0.25f, -height * 0.075f);
	menu->SetPosition(glm::vec3(0, height * -0.125f, 0));
	menu->SetCollder(new Collider(menu));

	closeButton->SetSize(width * 0.25f, -height * 0.075f);
	closeButton->SetPosition(glm::vec3(0, height * -0.25f, 0));
	closeButton->SetCollder(new Collider(closeButton));
}
void PauseWindow::Render() {
	if (display) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();
		renderer->Render(bgWindow);
		renderer->Render(setting);
		renderer->Render(load);
		renderer->Render(menu);
		renderer->Render(closeButton);
	}
}
void PauseWindow::LeftClick(float x, float y) {
	setting->checkColliderPressed(x, y);
	load->checkColliderPressed(x, y);
	menu->checkColliderPressed(x, y);
	closeButton->checkColliderPressed(x, y);
}
void PauseWindow::LeftRelease(float x, float y) {
	setting->checkColliderReleased(x, y);
	load->checkColliderReleased(x, y);
	menu->checkColliderReleased(x, y);
	closeButton->checkColliderReleased(x, y);
}
void PauseWindow::UpdateMouseButton(glm::vec3 screen) {
	setting->updateButton(screen.x, screen.y);
	load->updateButton(screen.x, screen.y);
	menu->updateButton(screen.x, screen.y);
	closeButton->updateButton(screen.x, screen.y);
}
void PauseWindow::Open() {
	Game* game = Game::GetInstance();
	game->GetCursor()->enableChange(false);
	game->GetScreen()->Pause = true;
	trigger = true;
}
void PauseWindow::Close() {
	Game* game = Game::GetInstance();
	game->GetCursor()->enableChange(true);
	game->GetScreen()->Pause = false;
	display = false;
}
PauseWindow::~PauseWindow() {
	delete setting;
	delete load;
	delete menu;
	delete closeButton;
}