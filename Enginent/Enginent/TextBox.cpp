#include "TextBox.h"
#include "Game.h"
#include "Player.h"

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
	std::string text = "";
	name->SetPosition(glm::vec3(-400.0f, -115.0f, 1.0f));
	choice_UI = new ChoiceUI();

	//use TTF_SizeText(TTF_Font *font, const char *text, int *w, int *h)
}

void TextBox::setText(std::string key, bool talk)
{
	Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_DIALOGUE_ON, true);
	SetDisplay(true);
	if (key != "")
	{
		Dialogue* tmp = scriptManager->GetDialogue(key);
		if (tmp->dialogue.size() != 0)
		{
			display = true;
			d_text = tmp;
			d_index = 0;
			name->loadText(d_text->dialogue[d_index].name, nameColor, 30);
			std::string tmp_text = d_text->dialogue[d_index].text;

			
			int charCount = 0;
			int lineCount = 0;
			for (int i = 0; i < dialogue.size(); i++)
			{
				delete dialogue[i];
			}
			dialogue.clear();
			while (tmp_text.size() > charCount)
			{
				std::stringstream s_tmp;
				for (int i = 0; i < MAX_CHAR && charCount < tmp_text.size(); i++)
				{
					if (tmp_text[charCount] == '\\')
					{
						charCount += 2;
						break;
					}
					s_tmp << tmp_text[charCount];
					charCount++;
				}
				TextObject* textObj = new TextObject();
				textObj->loadText(s_tmp.str(), textColor, 24);
				textObj->SetPosition(glm::vec3((-450 + (float)((s_tmp.str().size() * 10) / 2)), -180 - (lineCount * 24), 1.0f));
				dialogue.push_back(textObj);
				lineCount++;
			}

			GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
			//animation
			if (d_text->dialogue[d_index].animName != "")
			{
				Game::GetInstance()->GetPlayer()->anim->Play(d_text->dialogue[d_index].animName, false);
			}

			//note
			if (d_text->dialogue[d_index].noteName != "")
			{
				Phone* phone = Phone::GetInstance();
				phone->AddPage(NOTE, d_text->dialogue[d_index].noteName);
				SoundManager::GetInstance()->playSound(SFX, "CollectNote", false);
			}

			//puzzle
			if (d_text->dialogue[d_index].puzzleName != "")
			{
				((GameScreen*)Game::GetInstance()->GetScreen())->OpenPuzzle(d_text->dialogue[d_index].puzzleName);
			}


			Item* item = gs->FindItem(d_text->dialogue[d_index].itemName);
			InteractableObj* obj = dynamic_cast<InteractableObj*>(Game::GetInstance()->GetCurrentLevel()->GetCurrentRoom()->FindObject(d_text->dialogue[d_index].NPCName));
			if (item != nullptr && obj != nullptr && obj->hasItem)
			{
				gs->GetInventory()->AddItem(item);

				ViewWindow* vw = ViewWindow::GetInstance();
				vw->SetViewItem(item);
				vw->Open();
				obj->hasItem = false;
			}
		}
		if (tmp->changeNameObj.size() > 0)
		{
			for (std::map<std::string, ChangeName>::iterator it = tmp->changeNameObj.begin(); it != tmp->changeNameObj.end(); it++)
			{
				InteractableObj* obj = dynamic_cast<InteractableObj*>(Game::GetInstance()->GetCurrentLevel()->GetCurrentRoom()->FindObject(it->first));
				if (obj != nullptr)
				{
					obj->ChangeDialogue(it->second.d_before, it->second.d_after);
				}
			}
		}
	}
}

void TextBox::Render()
{
	if (display)
	{
		Game::GetInstance()->GetRenderer()->Render(background);
		Game::GetInstance()->GetRenderer()->Render(name);
		for (int i = 0; i < dialogue.size(); i++)
		{
			Game::GetInstance()->GetRenderer()->Render(dialogue[i]);
		}
	}
	if (choice_UI->IsDisplay())
	{
		choice_UI->Render();
	}
}

TextBox::~TextBox()
{
	for (int i = 0; i < dialogue.size(); i++)
	{
		delete dialogue[i];
	}
	delete background;
	delete choice_UI;
}

void TextBox::clickLeft(glm::vec3 pos)
{
	if (d_index < d_text->dialogue.size() - 1 && choice_UI->IsDisplay() == false)
	{
		d_index++;
		name->loadText(d_text->dialogue[d_index].name, nameColor, 30);
		std::string tmp_text = d_text->dialogue[d_index].text;

		int charCount = 0;
		int lineCount = 0;
		for (int i = 0; i < dialogue.size(); i++)
		{
			delete dialogue[i];
		}
		dialogue.clear();
		while (tmp_text.size() > charCount)
		{
			std::stringstream s_tmp;
			for (int i = 0; i < MAX_CHAR && charCount < tmp_text.size(); i++)
			{
				if (tmp_text[charCount] == '\\')
				{
					charCount += 2;
					break;
				}
				s_tmp << tmp_text[charCount];
				charCount++;
			}
			TextObject* textObj = new TextObject();
			textObj->loadText(s_tmp.str(), textColor, 24);
			textObj->SetPosition(glm::vec3((-450 + (float)((s_tmp.str().size() * 10) / 2)), -180 - (lineCount * 24), 1.0f));
			dialogue.push_back(textObj);
			lineCount++;
		}
		GameScreen* gs = ((GameScreen*)Game::GetInstance()->GetScreen());
		Item* item = gs->FindItem(d_text->dialogue[d_index].itemName);
		InteractableObj* obj = dynamic_cast<NonPlayer*>(Game::GetInstance()->GetCurrentLevel()->GetCurrentRoom()->FindObject(d_text->dialogue[d_index].NPCName));
		if (item != nullptr && obj != nullptr && obj->hasItem)
		{
			gs->GetInventory()->AddItem(item);

			ViewWindow* vw = ViewWindow::GetInstance();
			vw->SetViewItem(item);
			vw->Open();
			obj->hasItem = false;
		}
	}
	else if (choice_UI->IsDisplay())
	{
		choice_UI->clickLeft(pos);
	}
	else if (d_text->choice == "")
	{
		//Game::GetInstance()->GetCursor()->enableChange(true);
		Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_DIALOGUE_ON, false);
		display = false;
	}
	else
	{
		choice_UI->setChoice(d_text->choice);
	}
}

ChoiceBox::ChoiceBox()
{
	text = new TextObject();
	background = new UIObject();
	background->SetTexture("Texture/UI/ChoiceBox.PNG");
	background->SetSize(700, -50);
	background->SetCollder(new Collider(background));
}

void ChoiceBox::setChoice(Choice c)
{
	this->choice = c;
	text->loadText(choice.text, choiceColor, 24);
}

void ChoiceBox::SetPos(glm::vec3 pos)
{
	background->SetPosition(pos);
	text->SetPosition(pos);
	background->col->setRefObject(background);
}

void ChoiceBox::Render()
{
	Game::GetInstance()->GetRenderer()->Render(background);
	Game::GetInstance()->GetRenderer()->Render(text);
}

bool ChoiceBox::CheckClick(glm::vec3 pos)
{
	if (background->col->isClicked(pos.x, pos.y))
	{
		if(choice.nextScript != "")
			TextBox::GetInstance()->setText(choice.nextScript);
		else
		{
			TextBox::GetInstance()->SetDisplay(false);
		}
		return true;
	}
	return false;
}

ChoiceUI::ChoiceUI()
{
	display = false;
	scriptManager = ScriptManager::GetInstance();
	choiceNum = 0;
}

void ChoiceUI::setChoice(std::string key)
{
	std::vector<Choice>* choices = scriptManager->GetChoice(key);
	choiceNum = (int)choices->size();
	float first_Ypos = 50.0f + (((float)choiceNum / 2.0f) * 60.0f);
	if (choiceNum % 2 == 0)
	{
		first_Ypos += 0;
	}
	for (int i = 0; i < choiceNum; i++)
	{
		choiceList[i].setChoice((*choices)[i]);
		choiceList[i].SetPos(glm::vec3(0, first_Ypos - (i * 60), 0));
	}
	display = true;
}

void ChoiceUI::Render()
{
	if (display)
	{
		for (int i = 0; i < choiceNum; i++)
		{
			choiceList[i].Render();
		}
	}
}

void ChoiceUI::clickLeft(glm::vec3 pos)
{
	for (int i = 0; i < choiceNum; i++)
	{
		if (choiceList[i].CheckClick(pos))
		{
			display = false;
			break;
		}
	}
}