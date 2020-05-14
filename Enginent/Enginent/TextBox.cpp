#include "TextBox.h"
#include "Game.h"
#include "Player.h"
#include "TimeSystem.h"

#define TEXT_SPEED 100

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
	cutscene = new UIObject();
	cutscene->SetSize(1280, -720);

	//use TTF_SizeText(TTF_Font *font, const char *text, int *w, int *h)
}
void TextBox::setText(std::string key, bool talk)
{
	//std::cout << "key: " << key << std::endl;
	if (key != "")
	{
		Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_DIALOGUE_ON, true);
		SetDisplay(true);
		Dialogue* tmp = scriptManager->GetDialogue(key);
		if (tmp->dialogue.size() != 0)
		{
			d_text = tmp;
			d_index = 0;
			name->loadText(d_text->dialogue[d_index].name, nameColor, 30);
			std::stringstream tmp_text(d_text->dialogue[d_index].text);
			curr_dialogueSize = 0;
			showAll = false;

			int charCount = 0;
			int lineCount = 0;
			curr_dialogue.clear();
			this->charCount = 0;

			std::string word = "";
			std::stringstream s_tmp("");
			while(std::getline(tmp_text, word, ' '))
			{
				if (s_tmp.str().size() + word.size() > MAX_CHAR || word == "\\n")
				{
					std::string str(s_tmp.str());
					curr_dialogue.push_back(str);
					curr_dialogueSize += str.size() + 1;
					lineCount++;
					s_tmp.str("");
				}
				if(word != "\\n")
					s_tmp << " " << word;
			}
			std::string str(s_tmp.str());
			curr_dialogue.push_back(str);
			curr_dialogueSize += str.size();
			lineCount++;
			s_tmp.str("");

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

			//chat
			if (d_text->dialogue[d_index].chatName != "")
			{
				Phone* phone = Phone::GetInstance();
				phone->AddPage(CHAT, d_text->dialogue[d_index].chatName);
			}

			//puzzle
			if (d_text->dialogue[d_index].puzzleName != "")
			{
				((GameScreen*)Game::GetInstance()->GetScreen())->OpenPuzzle(d_text->dialogue[d_index].puzzleName);
			}

			//sound
			if (d_text->dialogue[d_index].soundName != "")
			{
				SoundManager::GetInstance()->playSound(SFX, d_text->dialogue[d_index].soundName);
			}

			//cutscene
			if (d_text->dialogue[d_index].CutScenebl)
			{
				cutscene->SetTexture(d_text->dialogue[d_index].CutScene);
				cutscene->SetDisplay(true);
			}
			else
			{
				cutscene->SetDisplay(false);
			}

			//change texture
			if (d_text->dialogue[d_index].spriteChange != "")
			{
				ImageObject* obj = dynamic_cast<ImageObject*>(Game::GetInstance()->GetCurrentLevel()->FindObject(d_text->dialogue[d_index].spriteChange));
				if (obj != nullptr)
				{
					obj->SetTexture(d_text->dialogue[d_index].sprite);
				}
				else
				{
					std::cout << "CHange sprite: Cannot find " << d_text->dialogue[d_index].spriteChange << std::endl;
				}
			}

			//change room
			if (d_text->dialogue[d_index].roomName != "")
			{
				Game::GetInstance()->GetCurrentLevel()->ChangeRoom(d_text->dialogue[d_index].roomName, d_text->dialogue[d_index].doorName);
			}

			//enable object
			if (d_text->dialogue[d_index].Enable != "")
			{
				Door* obj = dynamic_cast<Door*>(Game::GetInstance()->GetCurrentLevel()->FindObject(d_text->dialogue[d_index].Enable));
				if (obj != nullptr)
				{
					if (d_text->dialogue[d_index].en)
					{
						obj->Open();
					}
					else
					{
						obj->SetItemToUse("NONE");
					}
				}
				else
				{
					ImageObject* obj2 = dynamic_cast<ImageObject*>(Game::GetInstance()->GetCurrentLevel()->FindObject(d_text->dialogue[d_index].Enable));
					if (obj2 != nullptr)
					{
						obj2->SetDisplay(d_text->dialogue[d_index].en);
						obj2->col->enable = d_text->dialogue[d_index].en;
					}
				}
			}

			ViewWindow* vw = ViewWindow::GetInstance();
			if (d_text->dialogue[d_index].showItemWin)
			{
				vw->Open();
			}
			else
			{
				vw->Close();
			}


			Item* item = gs->FindItem(d_text->dialogue[d_index].itemName);
			InteractableObj* obj = nullptr;
			if(d_text->dialogue[d_index].NPCName != "")
				obj = dynamic_cast<InteractableObj*>(Game::GetInstance()->GetCurrentLevel()->FindObject(d_text->dialogue[d_index].NPCName));
			if (item != nullptr && ((obj != nullptr && obj->hasItem) || d_text->dialogue[d_index].NPCName == ""))
			{
				gs->GetInventory()->AddItem(item);

				vw->SetViewItem(item);
				vw->Open();
				if (obj != nullptr)
					obj->hasItem = false;
			}
		}
		if (tmp->changeNameObj.size() > 0)
		{
			for (std::map<std::string, ChangeName>::iterator it = tmp->changeNameObj.begin(); it != tmp->changeNameObj.end(); it++)
			{
				InteractableObj* obj = dynamic_cast<InteractableObj*>(Game::GetInstance()->GetCurrentLevel()->FindObject(it->first));
				if (obj != nullptr)
				{
					obj->ChangeDialogue(it->second.d_before, it->second.d_after);
				}
				else
				{
					std::cout << "Cannot find " << it->first << std::endl;
				}
			}
		}
	}
}

void TextBox::Render()
{
	if (display)
	{
		Update();
		Game::GetInstance()->GetRenderer()->Render(cutscene);
		Game::GetInstance()->GetRenderer()->Render(background);
		Game::GetInstance()->GetRenderer()->Render(name);
		for (int i = 0; i < dialogue.size(); i++)
		{
			Game::GetInstance()->GetRenderer()->Render(dialogue[i]);
		}
	}
	else {
		Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_DIALOGUE_ON, false);
	}
	if (choice_UI->IsDisplay())
	{
		choice_UI->Render();
	}
}

void TextBox::Update()
{
	time += TimeSystem::instance()->GetTimeBetweenFrame();
	if (time >= TEXT_SPEED && charCount <= curr_dialogueSize)
	{
		charCount++;
		time = 0;
	}

	if (!showAll)
	{
		for (int i = 0; i < dialogue.size(); i++)
		{
			delete dialogue[i];
		}
		dialogue.clear();
		int count = 0;
		int lineCount = 0;
		std::stringstream s_tmp("");
		for (int i = 0; i < charCount; i++)
		{
			if (count > curr_dialogue[lineCount].size())
			{
				TextObject* textObj = new TextObject();
				textObj->loadText(s_tmp.str(), textColor, 24);
				textObj->SetPosition(glm::vec3((-450 + (textObj->getSize().x / 2.0f)), -180 - (lineCount * 35), 1.0f));
				dialogue.push_back(textObj);
				s_tmp.str("");
				lineCount++;
				count = 0;
			}
			s_tmp << curr_dialogue[lineCount][count];
			count++;
		}
		TextObject* textObj = new TextObject();
		textObj->loadText(s_tmp.str(), textColor, 24);
		textObj->SetPosition(glm::vec3((-450 + (textObj->getSize().x / 2.0f)), -180 - (lineCount * 35), 1.0f));
		dialogue.push_back(textObj);
	}
	if (charCount >= curr_dialogueSize)
	{
		showAll = true;
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
	if (!showAll)
	{
		charCount = curr_dialogueSize - 1;
	}
	else if (d_index < d_text->dialogue.size() - 1 && choice_UI->IsDisplay() == false)
	{
		d_index++;
		name->loadText(d_text->dialogue[d_index].name, nameColor, 30);
		std::stringstream tmp_text(d_text->dialogue[d_index].text);
		curr_dialogueSize = 0;
		showAll = false;
		int charCount = 0;
		int lineCount = 0;
		std::string word = "";
		std::stringstream s_tmp("");
		curr_dialogue.clear();
		this->charCount = 0;
		while (std::getline(tmp_text, word, ' '))
		{
			if (s_tmp.str().size() + word.size() > MAX_CHAR || word == "\\n")
			{
				std::string str(s_tmp.str());
				curr_dialogue.push_back(str);
				curr_dialogueSize += str.size() + 1;
				lineCount++;
				s_tmp.str("");
			}
			if (word != "\\n")
				s_tmp << " " << word;
		}
		std::string str(s_tmp.str());
		curr_dialogue.push_back(str);
		curr_dialogueSize += str.size();
		lineCount++;
		s_tmp.str("");

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

		//chat
		if (d_text->dialogue[d_index].chatName != "")
		{
			Phone* phone = Phone::GetInstance();
			phone->AddPage(CHAT, d_text->dialogue[d_index].chatName);
		}

		//puzzle
		if (d_text->dialogue[d_index].puzzleName != "")
		{
			((GameScreen*)Game::GetInstance()->GetScreen())->OpenPuzzle(d_text->dialogue[d_index].puzzleName);
		}

		//sound
		if (d_text->dialogue[d_index].soundName != "")
		{
			SoundManager::GetInstance()->playSound(SFX, d_text->dialogue[d_index].soundName);
		}

		//cutscene
		if (d_text->dialogue[d_index].CutScenebl)
		{
			cutscene->SetTexture(d_text->dialogue[d_index].CutScene);
			cutscene->SetDisplay(true);
		}
		else
		{
			cutscene->SetDisplay(false);
		}

		//change texture
		if (d_text->dialogue[d_index].spriteChange != "")
		{
			ImageObject* obj = dynamic_cast<ImageObject*>(Game::GetInstance()->GetCurrentLevel()->FindObject(d_text->dialogue[d_index].spriteChange));
			if (obj != nullptr)
			{
				obj->SetTexture(d_text->dialogue[d_index].sprite);
			}
			else
			{
				std::cout << "CHange sprite: Cannot find " << d_text->dialogue[d_index].spriteChange << std::endl;
			}
		}

		//change room
		if (d_text->dialogue[d_index].roomName != "")
		{
			Game::GetInstance()->GetCurrentLevel()->ChangeRoom(d_text->dialogue[d_index].roomName, d_text->dialogue[d_index].doorName);
		}

		//enable object
			//enable object
		if (d_text->dialogue[d_index].Enable != "")
		{
			Door* obj = dynamic_cast<Door*>(Game::GetInstance()->GetCurrentLevel()->FindObject(d_text->dialogue[d_index].Enable));
			if (obj != nullptr)
			{
				if (d_text->dialogue[d_index].en)
				{
					obj->Open();
				}
				else
				{
					obj->SetItemToUse("NONE");
				}
			}
			else
			{
				ImageObject* obj2 = dynamic_cast<ImageObject*>(Game::GetInstance()->GetCurrentLevel()->FindObject(d_text->dialogue[d_index].Enable));
				if (obj2 != nullptr)
				{
					obj2->SetDisplay(d_text->dialogue[d_index].en);
					obj2->col->enable = d_text->dialogue[d_index].en;
				}
			}
		}

		

		ViewWindow* vw = ViewWindow::GetInstance();
		if (d_text->dialogue[d_index].showItemWin)
		{
			vw->Open();
		}
		else
		{
			vw->Close();
		}


		Item* item = gs->FindItem(d_text->dialogue[d_index].itemName);
		InteractableObj* obj = nullptr;
		if (d_text->dialogue[d_index].NPCName != "")
			obj = dynamic_cast<InteractableObj*>(Game::GetInstance()->GetCurrentLevel()->FindObject(d_text->dialogue[d_index].NPCName));
		if (item != nullptr && ((obj != nullptr && obj->hasItem) || d_text->dialogue[d_index].NPCName == ""))
		{
			gs->GetInventory()->AddItem(item);

			vw->SetViewItem(item);
			vw->Open();
			if (obj != nullptr)
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
		ViewWindow::GetInstance()->Close();
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
		if (choice.puzzleName != "")
		{
			((GameScreen*)Game::GetInstance()->GetScreen())->OpenPuzzle(choice.puzzleName);
		}
		if (choice.roomName != "")
		{
			((GameScreen*)Game::GetInstance()->GetScreen())->GetCurrentLevel()->ChangeRoom(choice.roomName, choice.doorName);
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