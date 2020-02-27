#include "InfoPhone.h"
#include "Game.h"

Note::Note(std::string n, unsigned int text) {
	name = n;
	texture = text;
}

void ChatInfo::AddText(std::string text) {
	texts.push_back(text);
	// calculate size of text and set position relative to the previous text
}

Chat::Chat() {
	profilePic = new UIObject();
	profilePic->SetSize(50.0f, -50.0f);
	profilePic->SetPosition(glm::vec3(-100.0f, 200.0f, 1));
	name = new TextObject();
}

Chat::~Chat() {
	delete profilePic;
}

void Chat::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(profilePic);
	renderer->Render(name);
	for (auto m : allMsg)
		renderer->Render(m);
}

void Chat::CloseChat() {
	for (auto m : allMsg)
		delete m;
}

void Chat::OpenChat(const ChatInfo c) {
	profilePic->SetTexture(c.picTexture);

	name->loadText(c.name, textColor, 24);
	name->SetPosition(glm::vec3(-100.0f + name->getSize().x * 0.5f, 200.0f, 1.0f));

	for (int i = 0; i < c.texts.size(); i++) {
		TextObject* tmpText = new TextObject();
		tmpText->loadText(c.texts[i], textColor, 24);
		tmpText->SetPosition(c.textPosition[i]);
		allMsg.push_back(tmpText);
	}
}

Application::Application(glm::vec3 phoneSize, glm::vec3 phonePos) {
	open = false;
	currentPage = 0;
	appBG = new UIObject();

	// init buttons
	float iconSize = phoneSize.x*0.25f;
	next = new PhoneNextButton("Texture/tmp_texture/tmp_arrowIcon.png");
	next->SetSize(iconSize, -iconSize);
	next->SetPosition(glm::vec3(phoneSize.x * 0.25f - phonePos.x, phonePos.y + phoneSize.y * -0.25f, 1));
	next->SetCollder(new Collider(next));

	back = new PhoneBackButton("Texture/tmp_texture/tmp_arrowIcon2.png");
	back->SetPosition(glm::vec3(-phoneSize.x * 0.25f - phonePos.x, phonePos.y + phoneSize.y * -0.25f, 1));
	back->SetSize(iconSize, -iconSize);
	back->SetCollder(new Collider(back));

	home = new PhoneHomeButton("Texture/tmp_texture/tmp_homeIcon.png");
	home->SetPosition(glm::vec3(phonePos.x, phonePos.y - phoneSize.y * 0.4f, 1));
	home->SetSize(iconSize, -iconSize);
	home->SetCollder(new Collider(home));
	
	buttons.push_back(next);
	buttons.push_back(back);
	buttons.push_back(home);

	currentNote = new UIObject();
	currentNote->SetDisplay(false);
	currentNote->SetSize(phoneSize.x, -phoneSize.y * 0.5f);

	currentChat = new Chat();
	// tmp chat
	ChatInfo tmpChat;
	float x = -100.0f;
	float y = 300.0f;
	tmpChat.name = "tmp_person";
	tmpChat.picTexture = Game::GetInstance()->GetRenderer()->LoadTexture("Texture/blankHeart.png");
	for (int i = 0; i < 5; i++) {
		tmpChat.texts.push_back(std::string(1,i));
		tmpChat.textPosition.push_back(glm::vec3(x, y, 1));
		y -= 50;
	}

	chats.push_back(tmpChat);
}

void Application::OpenChat() {
	currentChat->OpenChat(chats[currentPage]);
}

void Application::Clear() {
	for (auto n : notes)
		delete n;

	notes.clear();

	//for (auto c : chats)
	//	delete c.second;

	chats.clear();
}

void Application::LeftClick(float x, float y) {
	next->checkCollider(x, y);
	back->checkCollider(x, y);
	home->checkCollider(x, y);
}

void Application::Next() {
	currentPage++;
	switch (currentAppType)
	{
	case NOTE:
		if (currentPage == notes.size())
			currentPage--;
		break;
	case CHAT:
		if (currentPage == chats.size()) {
			currentPage--;
			OpenChat();
		}
		break;
	}
}

void Application::Back() {
	currentPage--;
	if (currentPage < 0)
		currentPage = 0;
	if(currentAppType == CHAT)
		OpenChat();
}

void Application::Render() {
	Game* game = Game::GetInstance();
	game->GetRenderer()->Render(appBG);

	switch (currentAppType)
	{
	case NOTE:
		if (notes.size() != 0)
			currentNote->SetTexture(notes[currentPage]->texture);
			game->GetRenderer()->Render(currentNote);
		break;
	case CHAT:
		if (chats.size() != 0)
			currentChat->Render();
		break;
	}

	game->GetRenderer()->Render(buttons);
}

void Application::AddNote(UIObject* obj) {
	currentNote->SetDisplay(true);
	notes.push_back(new Note(obj->object_name, obj->GetTexture()));
}

void Application::AddChat(std::string name) {
}

Application::~Application() {
	//for (auto c : chats)
	//	if (c.second)
	//		delete c.second;

	for (auto b : buttons)
		if (b)
			delete b;

	if (appBG)
		delete appBG;

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
	phone = new UIObject();
	phone->SetTexture("Texture/tmp_texture/tmp_phoneScreen.png"); //phone image
	float sizeX = 400.0f;
	float sizeY = 800.0f;
	phone->SetSize(sizeX, -sizeY);
	phone->SetPosition(glm::vec3(0, 0, 1));

	app = new Application(glm::vec3(sizeX, sizeY, 1), glm::vec3(0, 0, 1));
	notiChat = false;
	notiNote = false;
	open = false;

	float size = sizeX*.25f;
	// init all buttons
	noteIcon = new PhoneAppsButton("Texture/tmp_texture/tmp_noteIcon.png");
	noteIcon->SetSize(size, -size);
	noteIcon->SetPosition(glm::vec3(phone->getSize().x * 0.25f - phone->getPos().x, phone->getPos().y + phone->getSize().y * -0.25f, 1));
	noteIcon->SetApp(NOTE);
	noteIcon->SetCollder(new Collider(noteIcon));
	
	chatIcon = new PhoneAppsButton("Texture/tmp_texture/tmp_chatIcon.png");
	chatIcon->SetSize(size, -size);
	chatIcon->SetPosition(glm::vec3(-phone->getSize().x * 0.25f - phone->getPos().x, phone->getPos().y + phone->getSize().y * -0.25f, 1));
	chatIcon->SetApp(CHAT);
	chatIcon->SetCollder(new Collider(chatIcon));
	
	exitButton = new PhoneExitButton("Texture/tmp_texture/tmp_homeIcon.png");
	exitButton->SetSize(size, -size);
	exitButton->SetPosition(glm::vec3(phone->getPos().x, phone->getPos().y - phone->getSize().y * -0.4f, 1));
	exitButton->SetCollder(new Collider(exitButton));

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

	for (std::map<std::string, UIObject*>::iterator itr = notes.begin(); itr != notes.end(); itr++)
		if (itr->second)
			delete itr->second;
}

void Phone::Render() {
	Game::GetInstance()->GetRenderer()->Render(phone);
	if (app->open) {
		app->Render();
	}
	else {
		Game::GetInstance()->GetRenderer()->Render(icons);
	}
}

void Phone::LeftClick(float x, float y) {
	if (app->open) {
		app->LeftClick(x, y);
	}
	else {
		noteIcon->checkCollider(x, y);
		chatIcon->checkCollider(x, y);
		exitButton->checkCollider(x, y);
	}
}

void Phone::UpdateButton(float x, float y) {
	noteIcon->updateButton(x, y);
	chatIcon->updateButton(x, y);
	exitButton->updateButton(x, y);
}

void Phone::OpenApp(AppType apptype) {
	app->currentPage = 0;
	app->open = true;
	app->currentAppType = apptype;
	if (apptype == CHAT)
		app->OpenChat();
}

void Phone::CloseApp() {
	app->open = false;
}

void Phone::AddPage(AppType apptype, std::string name) {
	switch (apptype)
	{
	case NOTE: {
		app->AddNote(notes[name]);
		notiNote = false;
	}break;
	case CHAT:
		app->AddChat(name);
		notiChat = false;
	default:
		break;
	}
}

void Phone::SetNotification(AppType apptype) {
	switch (apptype)
	{
	case NOTE:
		notiNote = true;
		break;
	case CHAT: 
		notiChat = true;
	default:
		break;
	}
}

void Phone::Open() { 
	open = true; 
	Game::GetInstance()->GetCursor()->enableChange(false);
}
void Phone::Close() { 
	open = false; 
	Game::GetInstance()->GetCursor()->enableChange(true);
}