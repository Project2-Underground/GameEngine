#include "InfoPhone.h"
#include "Game.h"
#include <sstream>

void NoteInfo::AddText(std::string text) {
	std::stringstream ss(text);
	std::string modifiedText;
	std::string line;
	std::string word;
	lineCount = 1;
	while (ss >> word) {
		if (line.size() + word.size() > MAX_FONT_PER_LINE) {
			texts.push_back(line);
			textPosition.push_back(glm::vec3(TEXT_START_X, TEXT_TOP_Y, 0));
			line.clear();
			lineCount++;
		}
		line += word + " ";
	}
	if (line != "") {
		texts.push_back(line);
		textPosition.push_back(glm::vec3(TEXT_START_X, TEXT_TOP_Y, 0));
	}
}

void ChatInfo::AddText(std::string text) {
	//std::stringstream ss(text);
	//std::string modifiedText;
	//std::string line;
	//std::string word;
	//int lineCount = 1;
	//// add each words to line until length reach MAX_FONT_PER_LINE.
	//// when reach, add \n to line, add line to modifiedText.
	//// Then clear line to start adding new words.
	//while (ss >> word) {
	//	if (line.size() + word.size() > MAX_FONT_PER_LINE) {
	//		line += '\n';
	//		modifiedText += line;
	//		line.clear();
	//		lineCount++;
	//	}
	//	line += word + " ";
	//}
	//modifiedText += line;

	// start at text[nextLine], for loop backwards until we find the whitespace (' ').
	// replace that ' ' with '\n'.
	// continue to the next line by adding which is (nextLine + MAX_FONT_PER_LINE).
	// while loop until nextline > text size.

	int nextLine = MAX_FONT_PER_LINE;
	int lineCount = 1;
	while (text.size() > nextLine) {
		for (; text[nextLine] != ' '; nextLine--);
		text[nextLine] = '\n';
		lineCount++;
		nextLine += MAX_FONT_PER_LINE;
	}
	textPosition.push_back(glm::vec3(TEXT_START_X, TEXT_TOP_Y, 0));
	texts.push_back(text);
	lineCounts.push_back(lineCount);
	// calculate size of text and set position relative to the previous text
}

Note::Note() {
	scrollBar = new UIObject();
	thumb = new UIObject();
	scrollBar->SetTexture("Texture/tmp_texture/tmp_scrollBarBG.PNG");
	scrollBar->Init(10.0f, -(TEXT_TOP_Y - TEXT_BOTTOM_Y), glm::vec3(290, 0, 1));
	textBubble = new UIObject();
	textBubble->SetTexture("Texture/UI/InfoPhone/textBubble.png");
	textBubble->SetSize(575.0f, 250.0f);
	textBubble->SetPosition(glm::vec3(-20, scrollBar->getPos().y, 1));
	thumb->SetTexture("Texture/tmp_texture/tmp_scrollBar.PNG");
	thumb->SetSize(10.0f, -10.0f);
	thumb->SetPosition(glm::vec3(270, scrollBar->col->getMaxBound().y, 1));
	name = new TextObject();
	name->SetPosition(glm::vec3(-20, 175.0f, 1));
}
Note::~Note() {
	delete textBubble;
	delete scrollBar;
	delete thumb;
	delete name;
}
void Note::ClearText() {
	for (auto t : allMsg)
		delete t;
	allMsg.clear();
}
void Note::OpenNote(const NoteInfo n) {
	ClearText();

	name->loadText(n.name, { 255, 255, 255, 255 }, FONT_SIZE);

	float yPos = TEXT_TOP_Y;
	for (int i = 0; i < n.texts.size(); i++) {
		TextObject* tmpText = new TextObject();
		tmpText->loadText(n.texts[i], textColor, FONT_SIZE);

		// calculate starting x of the msg
		float paddingx = tmpText->getSize().x * 0.5f;

		// calculate starting y of the msg
		if (i > 0) {
			yPos -= (-tmpText->getSize().y * 0.5f + TEXT_SPACE + -allMsg.at(i - 1)->getSize().y * 0.5f);
		}
		else {
			yPos -= (-tmpText->getSize().y * 0.5f);
		}
		glm::vec3 newPos = glm::vec3(n.textPosition[i].x + paddingx, yPos, 0);
		tmpText->SetPosition(newPos);
		allMsg.push_back(tmpText);
	}
	if (n.lineCount > 0) {
		upperBound = allMsg.at(0)->getPos().y;
		lowerBound = -allMsg.at(allMsg.size() - 1)->getSize().y * 0.25f + TEXT_BOTTOM_Y;
	}
	// calculate thumb height
	float msgLength = (allMsg.at(0)->getPos().y - allMsg.at(allMsg.size() - 1)->getPos().y);
	float thumbY = ((scrollBar->getSize().y * scrollBar->getSize().y) / msgLength);
	float scrollNo = (msgLength - (upperBound - lowerBound)) / SCROLL_SPEED;

	if (scrollNo < 0) {
		thumb->SetSize(thumb->getSize().x, scrollBar->getSize().y);
		float space = -scrollBar->getSize().y - -thumb->getSize().y;

		scrollBarSpeed = 0;

		thumb->SetPosition(glm::vec3(scrollBar->getPos().x, scrollBar->getPos().y, 1));
	}
	else {
		thumb->SetSize(thumb->getSize().x, -thumbY);
		float space = -scrollBar->getSize().y - -thumb->getSize().y;

		scrollBarSpeed = space / scrollNo;

		thumb->SetPosition(glm::vec3(scrollBar->getPos().x, scrollBar->getPos().y + space * 0.5f, 1));
	}
}
void Note::Scroll(int direction) {
	if (direction < 0) {
		// scroll up
		if (allMsg.at(allMsg.size() - 1)->getPos().y + allMsg.at(allMsg.size() - 1)->getSize().y * -0.5f < lowerBound) {
			for (auto m : allMsg)
				m->SetPosition(glm::vec3(m->getPos().x, m->getPos().y + SCROLL_SPEED, 1));
			thumb->SetPosition(glm::vec3(thumb->getPos().x, thumb->getPos().y - scrollBarSpeed, 1));
		}
	}
	else {
		// scroll down
		if (allMsg.at(0)->getPos().y - allMsg.at(0)->getSize().y * -0.5f > upperBound) {
			for (auto m : allMsg)
				m->SetPosition(glm::vec3(m->getPos().x, m->getPos().y - SCROLL_SPEED, 1));
			thumb->SetPosition(glm::vec3(thumb->getPos().x, thumb->getPos().y + scrollBarSpeed, 1));
		}
	}
}
void Note::CloseNote() {
	for (auto m : allMsg)
		delete m;
}
void Note::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(name);
	//renderer->Render(textBubble);
	for (auto m : allMsg) {
		if (m->getPos().y < TEXT_TOP_Y && m->getPos().y > TEXT_BOTTOM_Y) {
			renderer->Render(m);
		}
	}
	renderer->Render(scrollBar);
	renderer->Render(thumb);
}

Chat::Chat() {
	profilePic = new UIObject();
	profilePic->SetSize(50.0f, -50.0f);
	profilePic->SetPosition(glm::vec3(TEXT_START_X, 175.0f, 1));
	textBubble = new UIObject();
	textBubble->SetTexture("Texture/UI/InfoPhone/textBubble.png");
	scrollBar = new UIObject();
	thumb = new UIObject();
	scrollBar->SetTexture("Texture/tmp_texture/tmp_scrollBarBG.PNG");
	scrollBar->Init(10.0f, -(TEXT_TOP_Y - TEXT_BOTTOM_Y), glm::vec3(290, (TEXT_TOP_Y + TEXT_BOTTOM_Y) * 0.5f, 1));
	thumb->SetTexture("Texture/tmp_texture/tmp_scrollBar.PNG");
	thumb->SetSize(10.0f, -10.0f);
	thumb->SetPosition(glm::vec3(270, scrollBar->col->getMaxBound().y, 1));
	name = new TextObject();
}

Chat::~Chat() {
	delete profilePic;
	delete textBubble;
	delete scrollBar;
	delete thumb;
	delete name;
}

void Chat::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(profilePic);
	renderer->Render(name);
	for (auto m : allMsg) {
		float sizeX = m->getSize().x + 20;
		float posX = m->getPos().x;
		if (sizeX > 580) 
			sizeX = 580;
		if (posX > -20)
			posX = -20;
		
		if (m->getPos().y < TEXT_TOP_Y && m->getPos().y > TEXT_BOTTOM_Y) {
			textBubble->SetSize(sizeX, m->getSize().y - 20);
			textBubble->SetPosition(glm::vec3(posX, m->getPos().y, 1));
			renderer->Render(textBubble);
			renderer->Render(m);
		}
	}
	renderer->Render(scrollBar);
	renderer->Render(thumb);
}

void Chat::CloseChat() {
	for (auto m : allMsg)
		delete m;
}

void Chat::ClearText() {
	for (auto t : allMsg)
		delete t;
	allMsg.clear();
}

void Chat::OpenChat(const ChatInfo c) {
	if (!Phone::GetInstance()->firstClose && Phone::GetInstance()->textAfterClose == "Phone_start" && c.name == "Unknown1") {
		Phone::GetInstance()->firstClose = true;
	}
	ClearText();
	profilePic->SetTexture(c.picTexture);

	name->loadText(c.name, { 255, 255, 255, 255 }, FONT_SIZE);
	name->SetPosition(glm::vec3(profilePic->getPos().x + 50.0f + name->getSize().x * 0.5f, 160.0f, 1.0f));

	float yPos = TEXT_TOP_Y;
	for (int i = 0; i < c.currentMsgIndex; i++) {
		TextObject* tmpText = new TextObject();
		tmpText->loadText(c.texts[i], textColor, FONT_SIZE);

		// calculate starting x of the msg
		float paddingx = tmpText->getSize().x * 0.5f;

		// calculate starting y of the msg
		if (i > 0) {
			yPos -= (-tmpText->getSize().y * 0.5f + TEXT_SPACE + -allMsg.at(i - 1)->getSize().y * 0.5f);
		}
		else {
			yPos -= (-tmpText->getSize().y * 0.5f);
		}
		glm::vec3 newPos = glm::vec3(c.textPosition[i].x + paddingx, yPos, 0);
		tmpText->SetPosition(newPos);
		allMsg.push_back(tmpText);
	}
	if (c.currentMsgIndex > 0) {
		upperBound = allMsg.at(0)->getPos().y;
		lowerBound = -allMsg.at(allMsg.size() - 1)->getSize().y*0.25f + TEXT_BOTTOM_Y;
	}
	// calculate thumb height
	float msgLength = (allMsg.at(0)->getPos().y - allMsg.at(allMsg.size() - 1)->getPos().y);
	thumb->SetSize(thumb->getSize().x, -((scrollBar->getSize().y * scrollBar->getSize().y) / msgLength));
	float space = -scrollBar->getSize().y - -thumb->getSize().y;

	float scrollNo = (msgLength - (upperBound - lowerBound)) / SCROLL_SPEED;
	scrollBarSpeed = space / scrollNo;
	thumb->SetPosition(glm::vec3(scrollBar->getPos().x, scrollBar->getPos().y + space * 0.5f, 1));
}

void Chat::Scroll(int direction) {
	if (direction < 0) {
		// scroll up
		if (allMsg.at(allMsg.size() - 1)->getPos().y + allMsg.at(allMsg.size() - 1)->getSize().y*-0.5f < lowerBound) {
			for (auto m : allMsg)
				m->SetPosition(glm::vec3(m->getPos().x, m->getPos().y + SCROLL_SPEED, 1));
			thumb->SetPosition(glm::vec3(thumb->getPos().x, thumb->getPos().y - scrollBarSpeed, 1));
		}
	}
	else {
		// scroll down
		if (allMsg.at(0)->getPos().y - allMsg.at(0)->getSize().y*-0.5f > upperBound) {
			for (auto m : allMsg)
				m->SetPosition(glm::vec3(m->getPos().x, m->getPos().y - SCROLL_SPEED, 1));
			thumb->SetPosition(glm::vec3(thumb->getPos().x, thumb->getPos().y + scrollBarSpeed, 1));
		}
	}
}

Application::Application() {
	open = false;
	currentPage = 0;
	appBG = new UIObject();

	// init buttons
	float iconSize = 100.0f;

	back = new PhoneBackButton("Texture/tmp_texture/tmp_arrowIcon2.png");
	back->Init(25, -25, glm::vec3(TEXT_START_X, TEXT_BOTTOM_Y - 35.0f, 1));
	back->col->setNewSize(100, -100);

	home = new PhoneHomeButton("");
	home->SetDisplay(false);
	home->Init(iconSize, -iconSize, glm::vec3(350, 0, 1));

	currentNote = new Note();

	currentChat = new Chat();

	notifyPopup = new UIObject();
	notifyPopup->SetTexture("Texture/tmp_texture/exclamationMark.png");
	notifyPopup->SetPosition(glm::vec3(TEXT_START_X, 0, 0));
	notifyPopup->SetSize(25.0f, -25.0f);
}

void Application::Open(AppType appType) {
	openNoteChat = false;
	open = true;
	currentAppType = appType;
	if (appType == CHAT)
		OpenChat();
	else
		OpenNote();
	if (!Unread(CHAT) && !Unread(NOTE))
		((GameScreen*)Game::GetInstance()->GetScreen())->phoneIcon->UpdateButton(false);
}

void Application::Close() {
	itemTabs.clear();
	openNoteChat = false;
	open = false;
}

void Application::OpenChat() {
	float y = TAB_TOP_Y;
	for (int i = 0; i < chats.size(); i++) {
		ChatNoteInfoButton* tab = new ChatNoteInfoButton("Texture/UI/InfoPhone/textBubble.png", chats[i]->name, i, chats[i]->noti);
		tab->Init(TAB_SIZE_X, -50.0f, glm::vec3(TEXT_START_X + TAB_SIZE_X * 0.5f, y - TAB_SPACE * i, 1.0f));
		tab->title->SetPosition(glm::vec3(TEXT_START_X + tab->title->getSize().x * 0.5f + TAB_TEXT_PADDING, y - TAB_SPACE * i, 1.0f));
		tab->hasNewInfo = chats[i]->noti;
		itemTabs.push_back(tab);
	}
	if (itemTabs.size() > 0) {
		upperBound = itemTabs.at(0)->getPos().y;
		lowerBound = -itemTabs.at(itemTabs.size() - 1)->getSize().y * 0.25f + TEXT_BOTTOM_Y;
	}
}

void Application::OpenNote() {
	float y = TAB_TOP_Y;
	for (int i = 0; i < notes.size(); i++) {
		ChatNoteInfoButton* tab = new ChatNoteInfoButton("Texture/UI/InfoPhone/textBubble.png", notes[i]->name, i, notes[i]->noti);
		tab->Init(TAB_SIZE_X, -50.0f, glm::vec3(TEXT_START_X + TAB_SIZE_X * 0.5f, y - TAB_SPACE * i, 1.0f));
		tab->title->SetPosition(glm::vec3(TEXT_START_X + tab->title->getSize().x * 0.5f + TAB_TEXT_PADDING, y - TAB_SPACE * i, 1.0f));
		itemTabs.push_back(tab);
	}
	if (itemTabs.size() > 0) {
		upperBound = itemTabs.at(0)->getPos().y;
		lowerBound = -itemTabs.at(itemTabs.size() - 1)->getSize().y * 0.25f + TEXT_BOTTOM_Y;
	}
}

bool Application::Unread(AppType apptype) {
	switch (apptype)
	{
	case NOTE:
		for (auto n : notes)
			if (n->noti)
				return true;
		break;
	case CHAT:
		for (auto c : chats)
			if (c->noti)
				return true;
		break;
	}
	return false;
}
bool Application::HasItem(AppType apptype, std::string name) {
	switch (apptype)
	{
	case NOTE:
		for (auto n : notes)
			if (n->name == name)
				return true;
		break;
	case CHAT:
		for (auto c : chats)
			if (c->name == name)
				return true;
		break;
	}
	return false;
}

void Application::Scroll(int direction) {
	if (!openNoteChat) {
		if (itemTabs.size() > 0) {
			if (direction < 0) {
				if (itemTabs.at(itemTabs.size() - 1)->getPos().y < lowerBound) {
					for (auto i : itemTabs) {
						i->SetPosition(glm::vec3(i->getPos().x, i->getPos().y + SCROLL_SPEED, 1.0f));
						i->title->SetPosition(glm::vec3(i->title->getPos().x, i->getPos().y, 1.0f));
					}
				}
			}
			else {
				if (itemTabs.at(0)->getPos().y > upperBound) {
					for (auto i : itemTabs) {
						i->SetPosition(glm::vec3(i->getPos().x, i->getPos().y - SCROLL_SPEED, 1.0f));
						i->title->SetPosition(glm::vec3(i->title->getPos().x, i->getPos().y, 1.0f));
					}
				}
			}
		}
	}
	else if (currentAppType == CHAT && chats.size() > 0)
		currentChat->Scroll(direction);
	else if (currentAppType == NOTE && notes.size() > 0)
		currentNote->Scroll(direction);

}

void Application::SelectItem(int index) {
	openNoteChat = true;
	switch (currentAppType)
	{
	case NOTE:
		notes[index]->noti = false;
		currentNote->OpenNote(*notes[index]);
		break;
	case CHAT:
		chats[index]->noti = false;
		currentChat->OpenChat(*chats[index]);
		break;
	}
}

void Application::Clear() {
	itemTabs.clear();
	notes.clear();
	chats.clear();
}

void Application::LeftClick(float x, float y) {
	for (auto i : itemTabs)
		i->checkColliderPressed(x, y);
	back->checkColliderPressed(x, y);
	home->checkColliderPressed(x, y);
}

void Application::LeftRelease(float x, float y) {
	for (auto i : itemTabs)
		i->checkColliderReleased(x, y);
	back->checkColliderReleased(x, y);
	home->checkColliderReleased(x, y);
}

void Application::Back() {
	if (openNoteChat)
		openNoteChat = false;
}

void Application::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(appBG);

	if (!openNoteChat) {
		for (auto b : itemTabs) {
			if (b->getPos().y - b->getSize().y * -0.25f <= upperBound &&
				b->getPos().y + b->getSize().y * -0.25f >= lowerBound) {
				renderer->Render(b);
				renderer->Render(b->title);
				if (b->hasNewInfo) {
					notifyPopup->SetPosition(glm::vec3(TEXT_START_X + b->getSize().x, b->getPos().y + -notifyPopup->getSize().y * 0.5f, 1.0f));
					renderer->Render(notifyPopup);
				}
			}
		}
	}
	else {
		switch (currentAppType)
		{
		case NOTE:
			currentNote->Render();
			break;
		case CHAT:
			currentChat->Render();
			break;
		}
		renderer->Render(back);
	}
	renderer->Render(home);
}

void Application::AddNote(NoteInfo* obj) {
	notes.push_back(obj);
}

void Application::AddChat(ChatInfo* chat) {
	chats.push_back(chat);
}

void Application::SetNote(NoteInfo* obj) {
	obj->noti = false;
	notes.push_back(obj);
}

void Application::SetChat(ChatInfo* chat) {
	chat->noti = false;
	chats.push_back(chat);
}

Application::~Application() {
	if (appBG)
		delete appBG;
	delete back;
	delete home;
	delete currentChat;
	delete currentNote;
}

Phone* Phone::_instance = nullptr;

Phone* Phone::GetInstance() {
	if (_instance == nullptr)
		_instance = new Phone();
	return _instance;
}

Phone::Phone() {
	textAfterClose = "Phone_start";
	phone = new UIObject();
	phone->SetTexture("Texture/UI/InfoPhone/Tablet.png");
	float sizeX = 825.0f;
	float sizeY = 500.5f;
	phone->SetSize(825.0f, -500.5f);
	phone->SetPosition(glm::vec3(0, 0, 1));

	app = new Application();
	open = false;

	float iconSize = 100.0f;
	// init all buttons
	noteIcon = new PhoneAppsButton("Texture/UI/InfoPhone/Note_app.png");
	noteIcon->SetPressTexture("Texture/UI/InfoPhone/Note_app_press.png");
	noteIcon->Init(iconSize, -iconSize, glm::vec3(-250.0f, 150.0f, 1));
	noteIcon->SetApp(NOTE);
	
	chatIcon = new PhoneAppsButton("Texture/UI/InfoPhone/Chat_app.png");
	chatIcon->SetPressTexture("Texture/UI/InfoPhone/Chat_app_press.png");
	chatIcon->Init(iconSize, -iconSize, glm::vec3(-100.0f, 150.0f, 1));
	chatIcon->SetApp(CHAT);
	
	exitButton = new PhoneExitButton("");
	exitButton->SetDisplay(false);
	exitButton->Init(iconSize, -iconSize, glm::vec3(350, 0, 1));

	notifyPopup = new UIObject();
	notifyPopup->SetTexture("Texture/tmp_texture/exclamationMark.png");
	notifyPopup->SetPosition(glm::vec3(0, chatIcon->getPos().y + -chatIcon->getSize().y * 0.5f, 0));
	notifyPopup->SetSize(iconSize*0.5f, -iconSize*0.5f);

	icons.push_back(noteIcon);
	icons.push_back(chatIcon);
	icons.push_back(exitButton);
}

Phone::~Phone() {
	for (auto ui : icons)
		if (ui)
			delete ui;
	if (phone)
		delete phone;
}

void Phone::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(phone);
	if (app->open) {
		app->Render();
	}
	else {
		renderer->Render(icons);
		if (app->Unread(CHAT)) {
			notifyPopup->SetPosition(glm::vec3(chatIcon->getPos().x + chatIcon->getSize().x * 0.5f, notifyPopup->getPos().y, 1.0f));
			renderer->Render(notifyPopup);
		}
		if (app->Unread(NOTE)) {
			notifyPopup->SetPosition(glm::vec3(noteIcon->getPos().x + noteIcon->getSize().x * 0.5f, notifyPopup->getPos().y, 1.0f));
			renderer->Render(notifyPopup);
		}
	}
	
}

void Phone::Scroll(glm::vec3 screen, int direction) {
	// check if mouse is within the phone screen
	app->Scroll(direction);
}

void Phone::LeftClick(float x, float y) {
	if (app->open) 
		app->LeftClick(x, y);
	else {
		noteIcon->checkColliderPressed(x, y);
		chatIcon->checkColliderPressed(x, y);
		exitButton->checkColliderPressed(x, y);
	}
}

void Phone::LeftRelease(float x, float y) {
	if (app->open)
		app->LeftRelease(x, y);
	else {
		noteIcon->checkColliderReleased(x, y);
		chatIcon->checkColliderReleased(x, y);
		exitButton->checkColliderReleased(x, y);
	}
}

void Phone::UpdateButton(float x, float y) {
	noteIcon->updateButton(x, y);
	chatIcon->updateButton(x, y);
	exitButton->updateButton(x, y);

}

void Phone::OpenApp(AppType apptype) {
	app->Open(apptype);
}

void Phone::CloseApp() {
	app->Close();
}

void Phone::AddPage(AppType apptype, std::string name) {
	if (!app->HasItem(apptype, name)) {
		((GameScreen*)Game::GetInstance()->GetScreen())->phoneIcon->UpdateButton(true);
		switch (apptype)
		{
		case NOTE: {
			app->AddNote(&notes[name]);
		}break;
		case CHAT:
			app->AddChat(&chats[name]);
		default:
			break;
		}
	}
}

void Phone::SetPage(AppType apptype, std::string name) {
	switch (apptype)
	{
	case NOTE: {
		app->SetNote(&notes[name]);
	}break;
	case CHAT:
		app->SetChat(&chats[name]);
	default:
		break;
	}
}

void Phone::Message(std::string name, int msgIndex) {
	((GameScreen*)Game::GetInstance()->GetScreen())->phoneIcon->UpdateButton(true);
	chats[name].currentMsgIndex = msgIndex;
	chats[name].noti = true;
	AddPage(CHAT, name);
}

void Phone::AddNPCInteracted(std::string name) {
	bool duplicate = false;
	for (auto n : npcTalked) {
		if (n == name) {
			duplicate = true;
			break;
		}
	}
	if (!duplicate)
		npcTalked.push_back(name);

	if (npcTalked.size() == 4 && !note23Done) {
		AddPage(NOTE, "Note2");
		AddPage(NOTE, "Note3");
		note23Done = true;
	}
}

void Phone::Clear() {
	app->Clear();
	note23Done = false;
	firstClose = false;
	textAfterClose = "Phone_start";
	npcTalked.clear();
}

void Phone::Open() { 
	open = true; 
	Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_PHONE_ON, true);
}
void Phone::Close() { 
	open = false; 
	Game::GetInstance()->GetCursor()->EnableCursor(CURSOR_PHONE_ON, false);
}

void Phone::PrintAllChat() {
	std::cout << chats.size();
	for (auto c : chats) {
		std::cout << c.first << ":\n";
		for (auto text : c.second.texts)
			std::cout << text;
		std::cout << std::endl;
	}
}