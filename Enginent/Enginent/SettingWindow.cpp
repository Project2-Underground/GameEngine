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

	masterVolUp = new SoundVolumeButton("", MASTER,1);
	masterVolDown = new SoundVolumeButton("", MASTER,-1);
	bgmVolUp = new SoundVolumeButton("",BGM,1);
	bgmVolDown = new SoundVolumeButton("",BGM,-1);
	sfxVolUp = new SoundVolumeButton("", SFX, 1);
	sfxVolDown = new SoundVolumeButton("", SFX, -1);
	masterMute = new SoundMuteButton("Texture/tmp_texture/m.png", MASTER);
	bgmMute = new SoundMuteButton("Texture/tmp_texture/b.png", BGM);
	sfxMute = new SoundMuteButton("Texture/tmp_texture/s.png", SFX);

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

	masterVolUp->Init(50.0f, -50.0f, glm::vec3(width * 0.125f, 200.0f, 1));
	/*masterVolUp->SetSize(50.0f, -50.0f);
	masterVolUp->SetPosition(glm::vec3(width * 0.125f, 200.0f, 1));
	masterVolUp->SetCollder(new Collider(masterVolUp));*/

	masterVolDown->SetSize(50.0f, -50.0f);
	masterVolDown->SetPosition(glm::vec3(-width * 0.125f, 200.0f, 1));
	masterVolDown->SetCollder(new Collider(masterVolDown));

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

	masterMute->SetSize(50.0f, -50.0f);
	masterMute->SetPosition(glm::vec3(60.0f, -50.0f, 1));
	masterMute->SetCollder(new Collider(masterMute));

	bgmMute->SetSize(50.0f, -50.0f);
	bgmMute->SetPosition(glm::vec3(0.0f, -50.0f, 1));
	bgmMute->SetCollder(new Collider(bgmMute));

	sfxMute->SetSize(50.0f, -50.0f);
	sfxMute->SetPosition(glm::vec3(-60.0f, -50.0f, 1));
	sfxMute->SetCollder(new Collider(sfxMute));

	closeButton->SetSize(bgWindow->getSize().x * 0.5f, bgWindow->getSize().y * 0.1f);
	closeButton->SetPosition(glm::vec3(bgWindow->getPos().x, bgWindow->getPos().y - -bgWindow->getSize().y * 0.5 + -closeButton->getSize().y, 0.0f));
	closeButton->SetCollder(new Collider(closeButton));
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
void SettingWindow::LeftClick(float x, float y) {
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
void SettingWindow::LeftRelease(float x , float y) {
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
void SettingWindow::UpdateMouseButton(glm::vec3 screen) {
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