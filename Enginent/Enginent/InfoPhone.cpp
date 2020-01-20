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

Application::Application() {
	open = false;
	currentPage = 0;
	appBG = new UIObject();

	// init buttons
	int size = 100;
	next = new PhoneNextButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	next->SetSize(size, -size);
	next->SetCollder(new Collider(next));

	back = new PhoneBackButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	back->SetSize(size, -size);
	back->SetCollder(new Collider(back));

	home = new PhoneHomeButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	home->SetSize(size, -size);
	home->SetCollder(new Collider(home));
	
	buttons.push_back(next);
	buttons.push_back(back);
	buttons.push_back(home);
	// init first note/chat or add later
}

void Application::Next() {
	switch (currentAppType)
	{
	case NOTE:
		if (notes.size() != 0)
			currentPage = (currentPage + 1) % notes.size();
		break;
	case CHAT:
		if (chats.size() != 0)
			currentPage = (currentPage + 1) % chats.size();
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

Phone* Phone::_instance = nullptr;

Phone* Phone::GetInstance() {
	if (_instance == nullptr)
		_instance = new Phone();
	return _instance;
}

Phone::Phone() {
	phone = new UIObject();
	phone->SetTexture("Texture/EliasRoom/Elias_Room_DoorAni.png"); //phone image
	phone->SetSize(200, -400);
	phone->SetPosition(glm::vec3(0, 0, 1));

	app = new Application();
	notiChat = false;
	notiNote = false;
	open = false;

	int size = 100;
	// init all buttons
	noteIcon = new PhoneAppsButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	noteIcon->SetSize(size, -size);
	noteIcon->SetPosition(glm::vec3(phone->getSize().x * 0.25 - phone->getPos().x, phone->getPos().y + phone->getSize().y * -0.25, 1));
	noteIcon->SetApp(NOTE);
	noteIcon->SetCollder(new Collider(noteIcon));
	
	chatIcon = new PhoneAppsButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	chatIcon->SetSize(size, -size);
	chatIcon->SetPosition(glm::vec3(-phone->getSize().x * 0.25 - phone->getPos().x, phone->getPos().y + phone->getSize().y * -0.25, 1));
	chatIcon->SetApp(CHAT);
	chatIcon->SetCollder(new Collider(chatIcon));
	
	exitButton = new PhoneExitButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	exitButton->SetSize(size, -size);
	exitButton->SetPosition(glm::vec3(phone->getPos().x, phone->getPos().y - phone->getSize().y * -0.25, 1));
	exitButton->SetCollder(new Collider(exitButton));

	icons.push_back(noteIcon);
	icons.push_back(chatIcon);
	icons.push_back(exitButton);
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

void Phone::LeftClick(int x, int y) {
	noteIcon->checkCollider(x, y);
	chatIcon->checkCollider(x, y);
	exitButton->checkCollider(x, y);
}

void Phone::UpdateButton(int x, int y) {
	noteIcon->updateButton(x, y);
	chatIcon->updateButton(x, y);
	exitButton->updateButton(x, y);
}

void Phone::OpenApp(AppType apptype) {
	app->open = true;
	app->currentAppType = apptype;
}

void Phone::CloseApp() {
	app->open = false;
}

void Phone::AddPage(AppType apptype, UIObject* page = nullptr, std::string name = "") {
	switch (apptype)
	{
	case NOTE:
		app->AddNote(page);
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