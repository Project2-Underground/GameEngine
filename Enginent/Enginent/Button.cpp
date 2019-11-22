#include "Button.h"
#include "Game.h"

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

//////////////Exit Button
Exit_Button::~Exit_Button()
{
	delete normalTexture;
	delete hoverTexture;
	delete pressTexture;
}

void Exit_Button::action(int x, int y)
{
	if(display)
	SDL_Quit();
}

//////////////Switch Scene Button
SwitchScene_Button::~SwitchScene_Button()
{
	delete normalTexture;
	delete hoverTexture;
	delete pressTexture;
}

void SwitchScene_Button::action(int x, int y)
{
	std::cout << "Change scene\n";
	for (int i = 0; i < t_screen.size(); i++)
	{
		t_screen[i]->setDisplay(false);
	}
	display = false;
	SoundManager::GetInstance()->stop("MainScreen");
	Game::GetInstance()->getPlayer()->setDisplay(true);
}