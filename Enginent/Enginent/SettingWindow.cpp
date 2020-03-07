#include "GameWindows.h"
#include "Game.h"

SettingWindow* SettingWindow::instance = nullptr;

SettingWindow* SettingWindow::GetInstance() {
	if (!instance)
		instance = new SettingWindow();
	return instance;
}
SettingWindow::SettingWindow() {
	bgWindow = new UIObject();
	bgWindow->SetTexture("Texture/tmp_texture/tmp_inventoryBox.png");
	display = false;
	trigger = false;

	bgmVolUp = new SoundVolumeButton("",BGM,1);
	bgmVolDown = new SoundVolumeButton("",BGM,-1);
	sfxVolUp = new SoundVolumeButton("", SOUNDFX, 1);
	sfxVolDown = new SoundVolumeButton("", SOUNDFX, -1);
	bgmmute = new SoundMuteButton("", BGM);
	sfxmute = new SoundMuteButton("", SOUNDFX);

	closeButton = new WindowCloseButton("Texture/tmp_texture/tmp_closeButton.png");
}
void SettingWindow::Update() {
	if (trigger) {
		display = true;
		trigger = false;
	}
}
void SettingWindow::Init(int width, int height) {
	bgWindow->SetSize((float)width * 0.5f, -(float)height * 0.5f);

	bgmVolUp->SetSize(50.0f, -50.0f);
	bgmVolUp->SetPosition(glm::vec3(width * 0.125f, 100.0f, 1));
	bgmVolUp->SetCollder(new Collider(bgmVolUp));

	bgmVolDown->SetSize(50.0f, -50.0f);
	bgmVolDown->SetPosition(glm::vec3(-width * 0.125f, 100.0f, 1));
	bgmVolDown->SetCollder(new Collider(bgmVolUp));

	sfxVolUp->SetSize(50.0f, -50.0f);
	sfxVolUp->SetPosition(glm::vec3(width * 0.125f, 0.0f, 1));
	sfxVolUp->SetCollder(new Collider(sfxVolUp));

	sfxVolDown->SetSize(50.0f, -50.0f);
	sfxVolDown->SetPosition(glm::vec3(-width * 0.125f, 0.0f, 1));
	sfxVolDown->SetCollder(new Collider(sfxVolDown));

	bgmmute->SetSize(50.0f, -50.0f);
	bgmmute->SetPosition(glm::vec3(40.0f, -50.0f, 1));
	bgmmute->SetCollder(new Collider(bgmmute));

	sfxmute->SetSize(50.0f, -50.0f);
	sfxmute->SetPosition(glm::vec3(-40.0f, -50.0f, 1));
	sfxmute->SetCollder(new Collider(sfxmute));

	closeButton->SetSize(bgWindow->getSize().x * 0.5f, bgWindow->getSize().y * 0.1f);
	closeButton->SetPosition(glm::vec3(bgWindow->getPos().x, bgWindow->getPos().y - -bgWindow->getSize().y * 0.5 + -closeButton->getSize().y, 0.0f));
	closeButton->SetCollder(new Collider(closeButton));
}
void SettingWindow::Render() {
	if (display) {
		GLRenderer* renderer = Game::GetInstance()->GetRenderer();

		renderer->Render(bgWindow);
		renderer->Render(bgmVolUp);
		renderer->Render(bgmVolDown);
		renderer->Render(sfxVolUp);
		renderer->Render(sfxVolDown);
		renderer->Render(bgmmute);
		renderer->Render(sfxmute);
		renderer->Render(closeButton);
	}
}
void SettingWindow::LeftClick(float x, float y) {
	bgmVolUp->checkColliderPressed(x, y);
	bgmVolDown->checkColliderPressed(x, y);
	sfxVolUp->checkColliderPressed(x, y);
	sfxVolDown->checkColliderPressed(x, y);
	bgmmute->checkColliderPressed(x, y);
	sfxmute->checkColliderPressed(x, y);
	closeButton->checkColliderPressed(x, y);
}
void SettingWindow::LeftRelease(float x , float y) {
	bgmVolUp->checkColliderReleased(x, y);
	bgmVolDown->checkColliderReleased(x, y);
	sfxVolUp->checkColliderReleased(x, y);
	sfxVolDown->checkColliderReleased(x, y);
	bgmmute->checkColliderReleased(x, y);
	sfxmute->checkColliderReleased(x, y);
	closeButton->checkColliderReleased(x, y);
}
void SettingWindow::UpdateMouseButton(glm::vec3 screen) {
	bgmVolUp->updateButton(screen.x, screen.y);
	bgmVolDown->updateButton(screen.x, screen.y);
	sfxVolUp->updateButton(screen.x, screen.y);
	sfxVolDown->updateButton(screen.x, screen.y);
	closeButton->updateButton(screen.x, screen.y);

}
SettingWindow::~SettingWindow() {
	delete bgmVolUp;
	delete bgmVolDown;
	delete bgmmute;
	delete sfxVolUp;
	delete sfxVolDown;
	delete sfxmute;
	delete closeButton;
	delete bgWindow;
}