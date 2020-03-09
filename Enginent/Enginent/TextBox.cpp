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
	display = false;
	scriptManager = ScriptManager::GetInstance();
	background = new UIObject();
	background->SetTexture("Texture/UI/Textbox.png");
	background->SetSize(1280, -720);
	background->SetPosition(glm::vec3(0, 0, 1.0f));
	name = new TextObject();
	name->loadText("", nameColor, 30);
	dialogue = new TextObject();
	std::string text = "";
	dialogue->loadText(text, textColor, 24);
	name->SetPosition(glm::vec3(-400.0f, -115.0f, 1.0f));

	//use TTF_SizeText(TTF_Font *font, const char *text, int *w, int *h)
	if(text.size() * 24 < 1000)
		dialogue->SetPosition(glm::vec3((-450 + (float)((text.size() * 24) / 2)), -180, 1.0f));
	else
		dialogue->SetPosition(glm::vec3((-450 + (float)(1000 / 2)), -180.0f, 1.0f));

}

void TextBox::setText(std::string key)
{
	display = true;
	d_text = scriptManager->GetDialogue(key);
	d_index = 0;
	std::cout << key << " here\n";
	name->loadText(d_text.dialogue[d_index].name, nameColor, 30);
	dialogue->loadText(d_text.dialogue[d_index].text, textColor, 24);
	dialogue->SetPosition(glm::vec3((-450 + (float)((d_text.dialogue[d_index].text.size() * 10) / 2)), -180, 1.0f));
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

void TextBox::clickLeft(glm::vec3 pos)
{
	if (d_index < d_text.dialogue.size() - 1)
	{
		d_index++;
		name->loadText(d_text.dialogue[d_index].name, nameColor, 30);
		dialogue->loadText(d_text.dialogue[d_index].text, textColor, 24);
		dialogue->SetPosition(glm::vec3((-450 + (float)((d_text.dialogue[d_index].text.size() * 10) / 2)), -180, 1.0f));
	}
	else
	{
		display = false;
	}
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