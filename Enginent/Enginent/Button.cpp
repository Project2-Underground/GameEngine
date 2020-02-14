#include "Button.h"
#include "Game.h"
#include "InfoPhone.h"
#include "GameViewWindow.h"
#include "MouseInput.h"

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

void Button::checkCollider(float x, float y)
{
	if (this->col->isClicked(x, y))
	{
		SetTexture(pressTexture);
		action();
	}
	else
	{
		SetTexture(normalTexture);
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


void PhoneOpenButton::action() {
	Phone::GetInstance()->Open();
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

void ClosePuzzleButton::action() {
	((GameScreen*)Game::GetInstance()->GetScreen())->ClosePuzzle();
}
