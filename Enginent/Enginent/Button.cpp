#include "Button.h"
#include "Game.h"
#include "InfoPhone.h"
#include "GameViewWindow.h"

Button::Button(std::string normal, std::string hover, std::string press)
{
	normalTexture = new ImageObject();
	normalTexture->SetTexture(normal);
	this->SetTexture(normalTexture->GetTexture());
	hoverTexture = new ImageObject();
	hoverTexture->SetTexture(hover);
	pressTexture = new ImageObject();
	pressTexture->SetTexture(press);
}

void Button::updateButton(int x, int y)
{
	if (this->col->isClicked((float)x, (float)y))
	{
		SetTexture(this->hoverTexture->GetTexture());
	}
	else
	{
		SetTexture(normalTexture->GetTexture());
	}
}

void Button::action(int x, int y)
{
}

void Button::checkCollider(int x, int y)
{
	if (this->col->isClicked((float)x, (float)y))
	{
		SetTexture(this->pressTexture->GetTexture());
		action(x, y);
	}
	else
	{
		this->SetTexture(normalTexture->GetTexture());
	}
}


Button::~Button() {
	delete normalTexture;
	delete hoverTexture;
	delete pressTexture;
}

//////////////Exit Button
void Exit_Button::action(int x, int y)
{
	if (display)
		Game::GetInstance()->quitGame();
}

//////////////Switch Scene Button
void SwitchScene_Button::action(int x, int y)
{
	Game::GetInstance()->ChangeScreenState(GAMESCREEN);
	SoundManager::GetInstance()->stop("MainScreen");
}

////////////// PhoneAppsButton
void PhoneAppsButton::action(int x, int y) {
	Phone::GetInstance()->OpenApp((AppType)appType);
}

void PhoneAppsButton::checkCollider(int x, int y) {
	if (this->col->isClicked((float)x, (float)y)) {
		action(x, y);
	}
}

void ActionButton::action(int x, int y) {
	Phone::GetInstance()->Open();
}

void ActionButton::checkCollider(int x, int y) {
	if (this->col->isClicked((float)x, (float)y)) {
		action(x, y);
	}
}

void PhoneExitButton::action(int x, int y) {
	Phone::GetInstance()->Close();
}

void PhoneNextButton::action(int x, int y) {
	Phone::GetInstance()->app->Next();
}

void PhoneBackButton::action(int x, int y) {
	Phone::GetInstance()->app->Back();
}

void PhoneHomeButton::action(int x, int y) {
	Phone::GetInstance()->CloseApp();
}

void ViewWindowClose::action(int x, int y) {
	ViewWindow::GetInstance()->SetDisplay(false);
}