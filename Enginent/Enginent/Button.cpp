#include "Button.h"
#include "Game.h"
#include "InfoPhone.h"
#include "GameViewWindow.h"
#include "MouseInput.h"

Button::Button(std::string normal, std::string hover, std::string press)
{
	SetActionTexture(normal, hover, press);
}

void Button::SetActionTexture(std::string normal, std::string hover, std::string press) {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	normalTexture = renderer->LoadTexture(normal);
	hoverTexture = renderer->LoadTexture(hover);
	pressTexture = renderer->LoadTexture(press);
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

void Button::checkCollider(float x, float y)
{
	if (this->col->isClicked(x, y))
	{
		SetTexture(pressTexture);
		action();
	}
	else
	{
		this->SetTexture(normalTexture);
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
	SoundManager::GetInstance()->stop("MainScreen");
}

////////////// PhoneAppsButton
void PhoneAppsButton::action() {
	Phone::GetInstance()->OpenApp((AppType)appType);
}

void PhoneAppsButton::checkCollider(float x, float y) {
	if (this->col->isClicked(x, y)) {
		action();
	}
}

void ActionButton::action() {
	Phone::GetInstance()->Open();
}

void ActionButton::checkCollider(float x, float y) {
	if (this->col->isClicked(x, y)) {
		action();
	}
}

void PhoneExitButton::action() {
	Phone::GetInstance()->Close();
}

void PhoneNextButton::action() {
	Phone::GetInstance()->app->Next();
}

void PhoneBackButton::action() {
	Phone::GetInstance()->app->Back();
}

void PhoneHomeButton::action() {
	Phone::GetInstance()->CloseApp();
}

void ViewWindowClose::action() {
	ViewWindow::GetInstance()->Close();
}
