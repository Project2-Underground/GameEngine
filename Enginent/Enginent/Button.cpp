#include "Button.h"
#include "Game.h"

Button::Button(std::string hover, std::string press)
{
	hoverTexture = new ImageObject();
	hoverTexture->SetTexture(hover);
	pressTexture = new ImageObject();
	pressTexture->SetTexture(press);
}

void Button::pressButton(int x, int y)
{
	UIObject::SetTexture(((Button*)pressTexture)->texture);
}

void Button::hoverCursor(int x, int y)
{
	this->SetTexture(((Button* )hoverTexture)->texture);
}

void Button::updateButton()
{

}

void Button::action(int x, int y)
{

}

//////////////Exit Button
void Exit_Button::action(int x, int y)
{
	SDL_Quit();
}

//////////////Switch Scene Button
SwitchScene_Button::SwitchScene_Button(std::string hover, std::string press)
{
	hoverTexture = new ImageObject();
	pressTexture = new ImageObject();
	this->hoverTexture->SetTexture(hover);
	this->pressTexture->SetTexture(press);
}

SwitchScene_Button::~SwitchScene_Button()
{
	delete hoverTexture;
	delete pressTexture;
}

void SwitchScene_Button::action(int x, int y)
{

}