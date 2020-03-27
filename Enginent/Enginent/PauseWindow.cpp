#include "Windows.h"
#include "Game.h"

PauseWindow* PauseWindow::instance = nullptr;

PauseWindow* PauseWindow::GetInstance() {
	if (!instance) {
		instance = new PauseWindow();
		Game* g = Game::GetInstance();
		instance->Init(g->winWidth, g->winHeight);
	}
	return instance;
}
PauseWindow::PauseWindow() {
	otherWindows.push_back(SettingWindow::GetInstance());
	otherWindows.push_back(SaveLoadWindow::GetInstance());

	bgWindow = new UIObject();
	bgWindow->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");

	setting = new SettingButton("Texture/tmp_texture/tmp_setting.png");
	setting->SetPressTexture("Texture/tmp_texture/tmp_setting_pressed.png");

	load = new OpenLoadSaveWindow("Texture/tmp_texture/tmp_load.png");
	load->SetPressTexture("Texture/tmp_texture/tmp_load_pressed.png");

	menu = new MainMenuButton("Texture/tmp_texture/tmp_mainmenu.png");
	menu->SetPressTexture("Texture/tmp_texture/tmp_mainmenu_pressed.png");

	closeButton = new PauseWindowCloseButton("Texture/tmp_texture/tmp_resume.png");
}
void PauseWindow::Update() {
	if (trigger) {
		display = true;
		trigger = false;
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
	bool otherWinOpen = false;
	for(auto win:otherWindows)
		if (win->IsOpen()) {
			win->Render();
			otherWinOpen = true;
			break;
		}
	if (display && !otherWinOpen) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();
		renderer->Render(bgWindow);
		renderer->Render(setting);
		renderer->Render(load);
		renderer->Render(menu);
		renderer->Render(closeButton);
	}
}
void PauseWindow::LeftClick(float x, float y) {
	bool otherWinOpen = false;
	for (auto win : otherWindows)
		if (win->IsOpen()) {
			win->LeftClick(x, y);
			otherWinOpen = true;
			break;
		}
	if (display && !otherWinOpen) {
		setting->checkColliderPressed(x, y);
		load->checkColliderPressed(x, y);
		menu->checkColliderPressed(x, y);
		closeButton->checkColliderPressed(x, y);
	}
}
void PauseWindow::LeftRelease(float x, float y) {
	bool otherWinOpen = false;
	for (auto win : otherWindows)
		if (win->IsOpen()) {
			win->LeftRelease(x, y);
			otherWinOpen = true;
			break;
		}
	if (display && !otherWinOpen) {
		setting->checkColliderReleased(x, y);
		load->checkColliderReleased(x, y);
		menu->checkColliderReleased(x, y);
		closeButton->checkColliderReleased(x, y);
	}
}
void PauseWindow::UpdateMouseButton(glm::vec3 screen) {
	bool otherWinOpen = false;
	for (auto win : otherWindows)
		if (win->IsOpen()) {
			win->UpdateMouseButton(screen);
			otherWinOpen = true;
			break;
		}
	if (display) {
		setting->updateButton(screen.x, screen.y);
		load->updateButton(screen.x, screen.y);
		menu->updateButton(screen.x, screen.y);
		closeButton->updateButton(screen.x, screen.y);
	}
}
void PauseWindow::Open() {
	Game* game = Game::GetInstance();
	game->GetCursor()->enableChange(false);
	game->GetScreen()->Pause = true;
	trigger = true;
	SoundManager::GetInstance()->pause(true);
}
void PauseWindow::Close() {
	Game* game = Game::GetInstance();
	game->GetCursor()->enableChange(true);
	game->GetScreen()->Pause = false;
	display = false;
	trigger = false;
	SoundManager::GetInstance()->pause(false);
}
PauseWindow::~PauseWindow() {
	delete setting;
	delete load;
	delete menu;
	delete closeButton;
}