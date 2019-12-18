#include "InfoPhone.h"
#include "Game.h"

Application::Application() {
	open = false;
	currentPage = 0;
	apps[NOTE] = notes;
	apps[CHAT] = chats;
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
	//SetTexture(); phone image
	app = nullptr;
	notiChat = false;
	notiNote = false;

	// init all buttons
	noteIcon;
	chatIcon;
	exitButton;

	icons.push_back(noteIcon);
	icons.push_back(chatIcon);
	icons.push_back(exitButton);
}

void Phone::Render() {
	Game::GetInstance()->GetRenderer()->Render(this);
	Game::GetInstance()->GetRenderer()->Render(icons);
	if (app->open)
		app->Render();
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