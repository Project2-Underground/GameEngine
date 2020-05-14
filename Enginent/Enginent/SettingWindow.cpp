#include "GameWindows.h"
#include "Game.h"

SettingWindow* SettingWindow::instance = nullptr;

SettingWindow* SettingWindow::GetInstance() {
	if (!instance){
		instance = new SettingWindow();
		Game* g = Game::GetInstance();
		instance->Init(g->winWidth, g->winHeight);
	}
	return instance;
}
SettingWindow::SettingWindow() {
	display = false;
	trigger = false;

	bgWindow = new UIObject();
	bgWindow->SetTexture("Texture/UI/GameUI/Sound_Menu.png");
	
	masterMute = new SoundMuteButton("Texture/UI/GameUI/MasterVolume_button2.png", MASTER);
	masterMute->SetMuteTexture("Texture/UI/GameUI/MasterVolume_button.png");
	bgmMute = new SoundMuteButton("Texture/UI/GameUI/BVolume_button2.png", BGM);
	bgmMute->SetMuteTexture("Texture/UI/GameUI/BVolume_button.png");
	sfxMute = new SoundMuteButton("Texture/UI/GameUI/SVolume_button2.png", SFX);
	sfxMute->SetMuteTexture("Texture/UI/GameUI/SVolume_button.png");
	
	closeButton = new SettingWindowCloseButton("Texture/UI/GameUI/ChatnameboxClose.png");
}
void SettingWindow::Init(int width, int height) {
	bgWindow->SetSize(487, -346);
	masterMute->Init(100.0f, -100.0f, glm::vec3(160.0f, 0, 1));
	bgmMute->Init(100.0f, -100.0f, glm::vec3(0.0f, 0, 1));
	sfxMute->Init(100.0f, -100.0f, glm::vec3(-160.0f, 0, 1));
	closeButton->Init(237, -44,
		glm::vec3(bgWindow->getPos().x, bgWindow->getPos().y - -bgWindow->getSize().y * 0.5 + -closeButton->getSize().y, 0.0f));


}
void SettingWindow::Render() {
	if (display) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();

		renderer->Render(bgWindow);
		renderer->Render(masterMute);
		renderer->Render(bgmMute);
		renderer->Render(sfxMute);
		renderer->Render(closeButton);
	}
}
void SettingWindow::Update() {
	if (trigger) {
		display = true;
		trigger = false;
	}
	if (display) {
		CursorUI* cursor = Game::GetInstance()->GetCursor();
	}
}
void SettingWindow::LeftClick(float x, float y) {
	if (display) {
		masterMute->checkColliderPressed(x, y);
		bgmMute->checkColliderPressed(x, y);
		sfxMute->checkColliderPressed(x, y);
		closeButton->checkColliderPressed(x, y);
	}
}
void SettingWindow::LeftRelease(float x , float y) {
	if (display) {
		masterMute->checkColliderReleased(x, y);
		bgmMute->checkColliderReleased(x, y);
		sfxMute->checkColliderReleased(x, y);
		closeButton->checkColliderReleased(x, y);
	}
}
void SettingWindow::UpdateMouseButton(glm::vec3 screen) {
	if (display) {
		masterMute->updateButton(screen.x, screen.y);
		bgmMute->updateButton(screen.x, screen.y);
		sfxMute->updateButton(screen.x, screen.y);
		closeButton->updateButton(screen.x, screen.y);
	}
}
SettingWindow::~SettingWindow() {
	//delete masterVolUp;
	//delete masterVolDown;
	//delete bgmVolUp;
	//delete bgmVolDown;
	//delete sfxVolUp;
	//delete sfxVolDown;
	delete masterMute;
	delete bgmMute;
	delete sfxMute;
	delete closeButton;
	delete bgWindow;
}