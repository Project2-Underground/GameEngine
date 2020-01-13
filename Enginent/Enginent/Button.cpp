#include "Button.h"
#include "Game.h"
#include "InfoPhone.h"

Button::Button(std::string normal, ::string hover, std::string press)
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
	if (this->col->isClicked(x, y))
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
	if (this->col->isClicked(x, y))
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


void PhoneButton::action(int x, int y) {
	Phone::GetInstance()->Open();
}