#include "InfoPhone.h"
#include "Game.h"

Application::Application() {
	open = false;
	currentPage = 0;
	apps[NOTE] = notes;
	apps[CHAT] = chats;

	//init first note/chat or add later
}

void Application::Next() {
	currentPage = (currentPage + 1) % apps[currentApp].size();
}

void Application::Back() {
	currentPage--;
	if (currentPage < 0)
		currentPage = 0;
}

void Application::Render() {
	Game::GetInstance()->GetRenderer()->Render(apps[currentApp].at(currentPage));
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
	phone->SetSize(100, 200);
	phone->SetPosition(glm::vec3(0, 0, 1));

	app = new Application();
	notiChat = false;
	notiNote = false;
	open = true;

	// init all buttons
	noteIcon = new PhoneAppsButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	noteIcon->SetSize(50, 50);
	noteIcon->SetPosition(glm::vec3(phone->getSize().x * 0.25 - phone->getPos().x, phone->getPos().y + phone->getSize().y * 0.25, 1));
	noteIcon->SetApp(NOTE);

	chatIcon = new PhoneAppsButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	chatIcon->SetSize(50, 50);
	chatIcon->SetPosition(glm::vec3(-phone->getSize().x * 0.25 - phone->getPos().x, phone->getPos().y + phone->getSize().y * 0.25, 1));
	chatIcon->SetApp(CHAT);

	exitButton = new PhoneAppsButton("Texture/EliasRoom/Elias_Room_DoorAni.png");
	exitButton->SetSize(50, 50);
	exitButton->SetPosition(glm::vec3(phone->getPos().x, phone->getPos().y - phone->getSize().y * 0.25, 1));

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

void Phone::ClickButton(int x, int y) {
	noteIcon->checkCollider(x, y);
	chatIcon->checkCollider(x, y);
	exitButton->checkCollider(x, y);
}

void Phone::UpdateButton(int x, int y) {
	noteIcon->updateButton(x, y);
	chatIcon->updateButton(x, y);
	exitButton->updateButton(x, y);
}

void Phone::Open() {
	open = true;
}

void Phone::Close() {
	open = false;
}

void Phone::OpenApp(AppType apptype) {
	app->open = true;
	app->currentApp = apptype;
}

void Phone::CloseApp() {
	app->open = false;
}

void Phone::AddPage(AppType apptype, UIObject* page) {
	app->apps[apptype].push_back(page);
	switch (apptype)
	{
	case NOTE:
		notiNote = false;
		break;
	case CHAT:
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