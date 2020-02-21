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
	glm::vec3 position = this->getPos();
	background = new UIObject();
	background->SetTexture("Texture/UI/Textbox.png");
	background->SetSize(1280, -720);
	background->SetPosition(position);
	name = new TextObject();
	name->loadText("Elias", nameColor, 30);
	name->SetPosition(glm::vec3(position.x - 400.0f, position.y - 115.0f, position.z));
	dialogue = new TextObject();
	dialogue->loadText("Test", textColor, 24);
	dialogue->SetPosition(glm::vec3(position.x - 0.0f, position.y - 180.0f, position.z));
}

void TextBox::setDialogue_vec(std::vector<Dialogue> d)
{
	this->display = true;
	this->dialogues = d;
}

void TextBox::setText(Dialogue d)
{
	this->display = true;
	this->name->loadText(d.name, nameColor, 30);
	this->dialogue->loadText(d.dialogue, textColor, 24);
	this->dialogueSize = d.dialogue.size();
	dialogue->SetPosition(glm::vec3((-450 + (float)((dialogueSize * 10) / 2)), -180, 0));
}

void TextBox::Render()
{
	this->SetTextboxPosition(Camera::GetInstance()->GetPosition());
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

void TextBox::SetTextboxPosition(glm::vec3 pos)
{
	name->SetPosition(glm::vec3(-(pos.x + 400.0f), pos.y - 115.0f, pos.z));
	dialogue->SetPosition(glm::vec3(-(pos.x - (-450 + (float)((dialogueSize * 10) / 2))), pos.y - 180.0f, pos.z));
}

TextBox::~TextBox()
{
	delete dialogue;
	delete background;
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