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
	bgWindow = new UIObject();
	bgWindow->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");
	display = false;
	trigger = false;

	masterVolUp = new SoundVolumeButton("Texture/tmp_texture/tmp_arrowIcon.png", MASTER,1);
	masterVolDown = new SoundVolumeButton("Texture/tmp_texture/tmp_arrowIcon2.png", MASTER,-1);
	bgmVolUp = new SoundVolumeButton("Texture/tmp_texture/tmp_arrowIcon.png",BGM,1);
	bgmVolDown = new SoundVolumeButton("Texture/tmp_texture/tmp_arrowIcon2.png",BGM,-1);
	sfxVolUp = new SoundVolumeButton("Texture/tmp_texture/tmp_arrowIcon.png", SFX, 1);
	sfxVolDown = new SoundVolumeButton("Texture/tmp_texture/tmp_arrowIcon2.png", SFX, -1);
	masterMute = new SoundMuteButton("Texture/tmp_texture/m.png", MASTER);
	masterMute->SetMuteTexture("Texture/tmp_texture/mMute.png");
	bgmMute = new SoundMuteButton("Texture/tmp_texture/b.png", BGM);
	bgmMute->SetMuteTexture("Texture/tmp_texture/bMute.png");
	sfxMute = new SoundMuteButton("Texture/tmp_texture/s.png", SFX);
	sfxMute->SetMuteTexture("Texture/tmp_texture/sMute.png");

	closeButton = new SettingWindowCloseButton("Texture/tmp_texture/tmp_closeButton.png");
}
void SettingWindow::Init(int width, int height) {
	bgWindow->SetSize((float)width * 0.5f, -(float)height * 0.5f);

	masterVolUp->Init(50.0f, -50.0f, glm::vec3(width * 0.125f, 200.0f, 1));
	masterVolDown->Init(50.0f, -50.0f, glm::vec3(-width * 0.125f, 200.0f, 1));
	bgmVolUp->Init(50.0f, -50.0f, glm::vec3(width * 0.125f, 100.0f, 1));
	bgmVolDown->Init(50.0f, -50.0f, glm::vec3(-width * 0.125f, 100.0f, 1));
	sfxVolUp->Init(50.0f, -50.0f, glm::vec3(width * 0.125f, 0.0f, 1));
	sfxVolDown->Init(50.0f, -50.0f, glm::vec3(-width * 0.125f, 0.0f, 1));
	masterMute->Init(50.0f, -50.0f, glm::vec3(60.0f, -50.0f, 1));
	bgmMute->Init(50.0f, -50.0f, glm::vec3(0.0f, -50.0f, 1));
	sfxMute->Init(50.0f, -50.0f, glm::vec3(-60.0f, -50.0f, 1));
	closeButton->Init(bgWindow->getSize().x * 0.5f, bgWindow->getSize().y * 0.1f, 
		glm::vec3(bgWindow->getPos().x, bgWindow->getPos().y - -bgWindow->getSize().y * 0.5 + -closeButton->getSize().y, 0.0f));


}
void SettingWindow::Render() {
	if (display) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();

		renderer->Render(bgWindow);
		renderer->Render(masterVolUp);
		renderer->Render(masterVolDown);
		renderer->Render(bgmVolUp);
		renderer->Render(bgmVolDown);
		renderer->Render(sfxVolUp);
		renderer->Render(sfxVolDown);
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
		cursor->enableChange(false);
	}
}
void SettingWindow::LeftClick(float x, float y) {
	if (display) {
		masterVolUp->checkColliderPressed(x, y);
		masterVolDown->checkColliderPressed(x, y);
		bgmVolUp->checkColliderPressed(x, y);
		bgmVolDown->checkColliderPressed(x, y);
		sfxVolUp->checkColliderPressed(x, y);
		sfxVolDown->checkColliderPressed(x, y);
		masterMute->checkColliderPressed(x, y);
		bgmMute->checkColliderPressed(x, y);
		sfxMute->checkColliderPressed(x, y);
		closeButton->checkColliderPressed(x, y);
	}
}
void SettingWindow::LeftRelease(float x , float y) {
	if (display) {
		masterVolUp->checkColliderReleased(x, y);
		masterVolDown->checkColliderReleased(x, y);
		bgmVolUp->checkColliderReleased(x, y);
		bgmVolDown->checkColliderReleased(x, y);
		sfxVolUp->checkColliderReleased(x, y);
		sfxVolDown->checkColliderReleased(x, y);
		masterMute->checkColliderReleased(x, y);
		bgmMute->checkColliderReleased(x, y);
		sfxMute->checkColliderReleased(x, y);
		closeButton->checkColliderReleased(x, y);
	}
}
void SettingWindow::UpdateMouseButton(glm::vec3 screen) {
	if (display) {
		masterVolUp->updateButton(screen.x, screen.y);
		masterVolDown->updateButton(screen.x, screen.y);
		bgmVolUp->updateButton(screen.x, screen.y);
		bgmVolDown->updateButton(screen.x, screen.y);
		sfxVolUp->updateButton(screen.x, screen.y);
		sfxVolDown->updateButton(screen.x, screen.y);
		masterMute->updateButton(screen.x, screen.y);
		bgmMute->updateButton(screen.x, screen.y);
		sfxMute->updateButton(screen.x, screen.y);
		closeButton->updateButton(screen.x, screen.y);
	}
}
SettingWindow::~SettingWindow() {
	delete masterVolUp;
	delete masterVolDown;
	delete bgmVolUp;
	delete bgmVolDown;
	delete sfxVolUp;
	delete sfxVolDown;
	delete masterMute;
	delete bgmMute;
	delete sfxMute;
	delete closeButton;
	delete bgWindow;
}