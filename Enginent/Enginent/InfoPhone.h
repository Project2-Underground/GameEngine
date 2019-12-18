#pragma once

#include "UIObject.h"
#include "Button.h"

enum AppType {
	NOTE = 0,
	CHAT
};

class Application {
public:
	Application();
	void Render();

	void Next();
	void Back();

	bool open;
	AppType currentApp;
	int currentPage;

	std::vector<UIObject*> notes;
	std::vector<UIObject*> chats;
	std::vector<UIObject*> apps[2];
private:
	Button* next;
	Button* back;
	Button* home;
};

class Phone : public UIObject {
public:
	static Phone* GetInstance();

	void Render();
	void ClickButton(int, int);
	void UpdateButton(int, int);

	void Open();
	void Close();
	void OpenApp(AppType);
	void CloseApp();
	void AddPage(AppType, UIObject*);
	void SetNotification(AppType);

	bool open;
private:
	std::vector<UIObject*> icons;

	Application* app;

	bool notiChat;
	bool notiNote;

	PhoneAppsButton* noteIcon;
	PhoneAppsButton* chatIcon;
	Button* exitButton;

	static Phone* _instance;
	Phone();
};