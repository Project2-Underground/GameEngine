#pragma once

#include "UIObject.h"
#include "Button.h"

enum AppType {
	NOTE = 0,
	CHAT
};

class Chat{
	std::string name;
	UIObject* pic;
	// bubble message
public:
	Chat(std::string);
	void LoadMessages(int index);
	void Render();
	// OnScroller
};

class Application {
public:
	Application();
	void Render();

	void Next();
	void Back();
	void AddNote(UIObject*);
	void AddChat(std::string);

	bool open;
	AppType currentAppType;
	int currentPage;

	std::vector<UIObject*> notes;
	std::vector<Chat*> chats;
private:
	Button* next;
	Button* back;
	Button* home;
};

class Phone{
public:
	static Phone* GetInstance();

	void Render();
	void ClickButton(int, int);
	void UpdateButton(int, int);

	void Open();
	void Close();
	void OpenApp(AppType);
	void CloseApp();
	void AddPage(AppType, UIObject*, std::string);
	void SetNotification(AppType);

	bool open;
private:
	std::vector<UIObject*> icons;

	Application* app;

	bool notiChat;
	bool notiNote;

	UIObject* phone;
	PhoneAppsButton* noteIcon;
	PhoneAppsButton* chatIcon;
	Button* exitButton;

	static Phone* _instance;
	Phone();
};