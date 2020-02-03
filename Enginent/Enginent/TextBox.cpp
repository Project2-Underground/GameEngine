#include "TextBox.h"
#include "Game.h"

TextBox::TextBox(int sizeX, int sizeY, int posX, int posY)
{
	background = new ImageObject();
	background->SetTexture("Texture/EliasRoom/Elias_Room_Door1.png");
	background->SetSize(1280.0f, 200.0f);
	background->SetPosition(glm::vec3(0.0f, -260.0f, 1.0f));
	dialogue = new TextObject();
	dialogue->loadText("Test", textColor, 20);
	dialogue->SetPosition(glm::vec3(0.0f, -260.0f, 1.0f));
}

void TextBox::setText(std::string name, std::string text)
{
	this->name->loadText(name, textColor, 24);
	this->dialogue->loadText(text, textColor, 20);
}

void TextBox::Render()
{
	if (display)
	{
		Game::GetInstance()->GetRenderer()->Render(background);
		Game::GetInstance()->GetRenderer()->Render(dialogue);
	}
}

void TextBox::toggleDisplay()
{
	display = !display;
}

TextBox::~TextBox()
{
	delete dialogue;
	delete background;
}

ChoiceBox::ChoiceBox()
{

}

void ChoiceBox::setText(std::string text)
{

}

ChoiceUI::ChoiceUI()
{

}