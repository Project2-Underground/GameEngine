#include "Button.h"
#include "Game.h"
#include "InfoPhone.h"
#include "GameWindows.h"
#include "MouseInput.h"
#include "SoundManager.h"

Button::Button(std::string texture)
{
	pressAvailable = true;
	togglePressed = false;
	SetTexture(texture);
	normalTexture = GetTexture();
	hoverTexture = normalTexture;
	pressTexture = normalTexture;
}

void Button::SetHoverTexture(std::string texture) {
	hoverTexture = Game::GetInstance()->GetRenderer()->LoadTexture(texture);
}

void Button::SetPressTexture(std::string texture) {
	pressTexture = Game::GetInstance()->GetRenderer()->LoadTexture(texture);
}

void Button::updateButton(float x, float y)
{
	if (this->col->isClicked(x, y))
	{
		SetTexture(hoverTexture);
	}
	else
	{
		SetTexture(normalTexture);
	}
}

void Button::action()
{
	//does something
}

void Button::checkColliderPressed(float x, float y)
{
	if (this->col->isClicked(x, y)) {
		SetTexture(pressTexture);
		MouseInput::GetInstance()->SetCurrentButtonPressed(this);
		Game::GetInstance()->GetScreen()->buttonClicked = true;
	}
}

void Button::checkColliderReleased(float x, float y) {
	if (this->col->isClicked(x, y)) {
		if (MouseInput::GetInstance()->GetCurrentButtonPressed() == this) {
			MouseInput::GetInstance()->SetCurrentButtonPressed(nullptr);
			action();
		}
		SetTexture(this->normalTexture);
	}
}

//////////////Exit Button
void Exit_Button::action()
{
	if (display)
		Game::GetInstance()->quitGame();
}

//////////////Switch Scene Button
void SwitchScene_Button::action()
{
	Game::GetInstance()->ChangeScreenState(GAMESCREEN);
	Game::GetInstance()->GetScreen()->CloseGameAllWindow();
	SoundManager::GetInstance()->stop(BGM, "MainScreen");
}


void OpenLoadSaveWindow::action() {
	Game::GetInstance()->SetSaveGame(false);
	SaveLoadWindow::GetInstance()->Open();
}

////////////// PhoneAppsButton
void PhoneAppsButton::action() {
	Phone::GetInstance()->OpenApp((AppType)appType);
}

void PhoneOpenButton::action() {
	Phone::GetInstance()->Open();
}

void PhoneExitButton::action() {
	Phone::GetInstance()->Close();
}
//
//void PhoneNextButton::action() {
//	Phone::GetInstance()->app->Next();
//}

void PhoneBackButton::action() {
	Phone::GetInstance()->app->Back();
}

void PhoneHomeButton::action() {
	Phone::GetInstance()->CloseApp();
}

void ViewWindowCloseButton::action() {
	ViewWindow::GetInstance()->Close();
}
void SaveLoadWindowCloseButton::action() {
	SaveLoadWindow::GetInstance()->Close();
}
void PauseWindowCloseButton::action() {
	PauseWindow::GetInstance()->Close();
}
void SettingWindowCloseButton::action() {
	SettingWindow::GetInstance()->Close();
}

void ClosePuzzleButton::action() {
	((GameScreen*)Game::GetInstance()->GetScreen())->ClosePuzzle();
}

void SaveLoadGameButton::action() {
	SoundManager::GetInstance()->stopAllSounds();
	Game::GetInstance()->SaveLoad(filename);
}

void OpenPauseWindowButton::action() {
	PauseWindow::GetInstance()->Open();
	// pause sfx
}

void MainMenuButton::action() {
	Game* game = Game::GetInstance();
	game->GetScreen()->CloseGameAllWindow();
	game->ChangeScreenState(MENUSCREEN);
	SoundManager::GetInstance()->stopAllSounds();

}

void SettingButton::action(){
	SettingWindow::GetInstance()->Open();
}

void SoundVolumeButton::action() {
	if (direction >= 0)
		SoundManager::GetInstance()->upVolume((SoundType)type);
	else
		SoundManager::GetInstance()->downVolume((SoundType)type);
	
}

void SoundMuteButton::SetMuteTexture(std::string texture){
	muteTexture = Game::GetInstance()->GetRenderer()->LoadTexture(texture);
}
void SoundMuteButton::action() {
	if (GetTexture() != muteTexture)
		SetTexture(muteTexture);
	else
		SetTexture(normalTexture);

	SoundManager::GetInstance()->toggleMute((SoundType)type);
}

void SoundMuteButton::updateButton(float x, float y) {
	if (this->col->isClicked(x, y))
	{
		SetTexture(hoverTexture);
	}
	else
	{
		if(SoundManager::GetInstance()->getMute((SoundType)type))
			SetTexture(muteTexture);
		else 
			SetTexture(normalTexture);
	}
}

ChatNoteInfoButton::ChatNoteInfoButton(std::string texture, std::string _title, int _index, bool noti) :Button(texture) {
	hasNewInfo = noti;
	index = _index;
	title = new TextObject();
	title->loadText(_title, textColor, 18); 
}

void ChatNoteInfoButton::action() {
	hasNewInfo = false;
	Phone::GetInstance()->app->SelectItem(index);
}