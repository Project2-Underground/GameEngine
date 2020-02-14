#include "TextBox.h"
#include "Game.h"

TextBox* TextBox::_instance = nullptr;

TextBox* TextBox::GetInstance() {
	if (_instance == nullptr)
		_instance = new TextBox();
	return _instance;
}

TextBox::TextBox()
{
	background = new ImageObject();
	background->SetTexture("Texture/UI/Textbox.png");
	background->SetSize(1280, -720);
	background->SetPosition(glm::vec3(0, 0, 1.0f));
	name = new TextObject();
	name->loadText("Elias", nameColor, 30);
	name->SetPosition(glm::vec3(-400.0f, -115.0f, 1.0f));
	dialogue = new TextObject();
	dialogue->loadText("Test", textColor, 24);
	dialogue->SetPosition(glm::vec3(0.0f, -180.0f, 1.0f));
}

void TextBox::setText(std::vector<Dialogue> d)
{
	this->display = true;
	this->dialogues = d;
}

void TextBox::setText(Dialogue d)
{
	this->display = true;
	this->name->loadText(d.name, nameColor, 30);
	this->dialogue->loadText(d.dialogue, textColor, 24);
	dialogue->SetPosition(glm::vec3(0, -180, 0));
}

void TextBox::Render()
{
	if (display)
	{
		Game::GetInstance()->GetRenderer()->Render(background);
		Game::GetInstance()->GetRenderer()->Render(name);
		Game::GetInstance()->GetRenderer()->Render(dialogue);
	}
}

void TextBox::setTextboxDisplay(bool b)
{
	display = b;
}

TextBox::~TextBox()
{
	delete dialogue;
	delete background;
}

std::vector<Dialogue> TextBox::GetDialogueList()
{
	return dialogues;
}

void TextBox::clickLeft(glm::vec3 pos)
{
	
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