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
	bgWindow->SetTexture("Texture/tmp_texture/tmp_pausebg.png");

	setting = new SettingButton("Texture/UI/GameUI/Setting_button.png");
	setting->SetPressTexture("Texture/UI/GameUI/Setting_button_press.png");

	load = new OpenLoadSaveWindow("Texture/UI/GameUI/Loading_button.png");
	load->SetPressTexture("Texture/UI/GameUI/Loading_button_press.png");

	menu = new MainMenuButton("Texture/UI/GameUI/Main_menu_button.png");
	menu->SetPressTexture("Texture/UI/GameUI/Main_menu_button_press.png");

	closeButton = new PauseWindowCloseButton("Texture/UI/GameUI/Resume_button.png");
	closeButton->SetPressTexture("Texture/UI/GameUI/Resume_button_press.png");
}
void PauseWindow::Init(int width, int height) {
	bgWindow->SetSize((float)width, -(float)height);

	setting->Init(257, -44, glm::vec3(0, height * 0.25f, 0));
	load->Init(167, -44, glm::vec3(0, height * 0.125f, 0));
	menu->Init(316, -44, glm::vec3(0, height * -0.125f, 0));
	closeButton->Init(237, -44, glm::vec3(0, height * -0.25f, 0));
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
	else if (otherWinOpen) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();
		renderer->Render(bgWindow);
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
	//game->GetCursor()->enableChange(false);
	game->GetCursor()->EnableCursor(CURSOR_PAUSE_ON, true);
	game->GetScreen()->Pause = true;
	trigger = true;
	SoundManager::GetInstance()->pause(true);
}
void PauseWindow::Close() {
	Game* game = Game::GetInstance();
	//game->GetCursor()->enableChange(true);
	game->GetCursor()->EnableCursor(CURSOR_PAUSE_ON, false);
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