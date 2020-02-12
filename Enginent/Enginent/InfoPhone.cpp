#include "InfoPhone.h"
#include "Game.h"

Chat::Chat(std::string name) {
	// load chat from xml
	this->name = name;
	currentMsgIndex = 0;
	pic = new UIObject();
	XMLManager::GetInstance()->GetChat(name,pic);
}

Chat::~Chat() {
	delete pic;
}

void Chat::LoadMessages(int number) {
	// load next number of messages
	XMLManager* lg = XMLManager::GetInstance();
	for (int i = currentMsgIndex; i < currentMsgIndex + number; i++) {
		// store messages in texts
		texts.push_back(lg->GetMessage(name, i));
	}
	currentMsgIndex += number;
}

void Chat::Render() {
	Game::GetInstance()->GetRenderer()->Render(pic);
	// render text bubble
}

Application::Application(glm::vec3 phoneSize, glm::vec3 phonePos) {
	open = false;
	currentPage = 0;
	appBG = new UIObject();

	// init buttons
	float iconSize = phoneSize.x*0.25f;
	next = new PhoneNextButton("Texture/tmp_inventoryBox.png");
	next->SetSize(iconSize, -iconSize);
	next->SetPosition(glm::vec3(phoneSize.x * 0.25 - phonePos.x, phonePos.y + phoneSize.y * -0.25, 1));
	next->SetCollder(new Collider(next));

	back = new PhoneBackButton("Texture/tmp_inventoryBox.png");
	back->SetPosition(glm::vec3(-phoneSize.x * 0.25 - phonePos.x, phonePos.y + phoneSize.y * -0.25, 1));
	back->SetSize(iconSize, -iconSize);
	back->SetCollder(new Collider(back));

	home = new PhoneHomeButton("Texture/tmp_inventoryBox.png");
	home->SetPosition(glm::vec3(phonePos.x, phonePos.y - phoneSize.y * 0.25, 1));
	home->SetSize(iconSize, -iconSize);
	home->SetCollder(new Collider(home));
	
	buttons.push_back(next);
	buttons.push_back(back);
	buttons.push_back(home);
	// init first note/chat or add later
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
		if (currentPage == chats.size())
			currentPage--;
		break;
	}
}

void Application::Back() {
	currentPage--;
	if (currentPage < 0)
		currentPage = 0;
}

void Application::Render() {
	Game* game = Game::GetInstance();
	game->GetRenderer()->Render(appBG);

	switch (currentAppType)
	{
	case NOTE:
		if (notes.size() != 0)
			game->GetRenderer()->Render(notes.at(currentPage));
		break;
	case CHAT:
		if (chats.size() != 0)
			chats.at(currentPage)->Render();
		break;
	}

	game->GetRenderer()->Render(buttons);
}

void Application::AddNote(UIObject* obj) {
	notes.push_back(obj);
}

void Application::AddChat(std::string name) {
	// pull chat info from xml file
	chats.push_back(new Chat(name));
}

Application::~Application() {
	for (auto c : chats)
		if (c)
			delete c;

	for (auto b : buttons)
		if (b)
			delete b;

	if (appBG)
		delete appBG;
}

Phone* Phone::_instance = nullptr;

Phone* Phone::GetInstance() {
	if (_instance == nullptr)
		_instance = new Phone();
	return _instance;
}

Phone::Phone() {
	phone = new UIObject();
	phone->SetTexture("Texture/tmp_phoneScreen.png"); //phone image
	float sizeX = 200.0f;
	float sizeY = 400.0f;
	phone->SetSize(sizeX, -sizeY);
	phone->SetPosition(glm::vec3(0, 0, 1));

	app = new Application(glm::vec3(sizeX, sizeY, 1), glm::vec3(0, 0, 1));
	UIObject* tmp_note = new UIObject();
	tmp_note->SetSize(sizeX, -sizeY*0.5f);
	tmp_note->SetTexture("Texture/idle_test.png");
	app->AddNote(tmp_note);
	tmp_note = new UIObject();
	tmp_note->SetSize(sizeX, -sizeY*0.5f);
	tmp_note->SetTexture("Texture/idle_test2.png");
	app->AddNote(tmp_note);
	notiChat = false;
	notiNote = false;
	open = false;

	float size = 100.0f;
	// init all buttons
	noteIcon = new PhoneAppsButton("Texture/tmp_inventoryBox.png");
	noteIcon->SetSize(size, -size);
	noteIcon->SetPosition(glm::vec3(phone->getSize().x * 0.25 - phone->getPos().x, phone->getPos().y + phone->getSize().y * -0.25, 1));
	noteIcon->SetApp(NOTE);
	noteIcon->SetCollder(new Collider(noteIcon));
	
	chatIcon = new PhoneAppsButton("Texture/tmp_inventoryBox.png");
	chatIcon->SetSize(size, -size);
	chatIcon->SetPosition(glm::vec3(-phone->getSize().x * 0.25 - phone->getPos().x, phone->getPos().y + phone->getSize().y * -0.25, 1));
	chatIcon->SetApp(CHAT);
	chatIcon->SetCollder(new Collider(chatIcon));
	
	exitButton = new PhoneExitButton("Texture/tmp_inventoryBox.png");
	exitButton->SetSize(size, -size);
	exitButton->SetPosition(glm::vec3(phone->getPos().x, phone->getPos().y - phone->getSize().y * -0.25, 1));
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
}

void Phone::CloseApp() {
	app->open = false;
}

void Phone::AddPage(AppType apptype, std::string name) {
	switch (apptype)
	{
	case NOTE:
		app->AddNote(notes[name]);
		notiNote = false;
		break;
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