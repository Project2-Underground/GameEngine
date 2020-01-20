#pragma once

#include "UIObject.h"
#include "Button.h"

enum AppType {
	NOTE = 0,
	CHAT
};

class Chat{
	std::string name;					// name of the chat
	UIObject* pic;						// profile picture (?)
	int currentMsgIndex;				// show up to this message
	std::vector<std::string> texts;		// store all messages from the chat
	// bubble message
public:
	~Chat();
	Chat(std::string);
	void LoadMessages(int index);
	void Render();
	// OnScroller
};

class Application {
public:
	Application(glm::vec3 phoneSize, glm::vec3 phonePos);
	void Render();

	void LeftClick(int, int);
	void Next();
	void Back();
	void AddNote(UIObject*);
	void AddChat(std::string);

	bool open;
	AppType currentAppType;
	int currentPage;

	std::vector<UIObject*> notes;
	std::vector<Chat*> chats;
	std::vector<UIObject*> buttons;
private:
	UIObject* appBG;
	PhoneNextButton* next;
	PhoneBackButton* back;
	PhoneHomeButton* home;
};

class Phone{
public:
	static Phone* GetInstance();

	void Render();
	void LeftClick(int, int);
	void UpdateButton(int, int);

	void OpenApp(AppType);
	void CloseApp();
	void AddPage(AppType, UIObject*, std::string);
	void SetNotification(AppType);
	void Open() { open = true; }
	void Close() { open = false; }

	bool open;
	Application* app;
private:
	Phone();
	std::vector<UIObject*> icons;

	bool notiChat;
	bool notiNote;

	UIObject* phone;
	PhoneAppsButton* noteIcon;
	PhoneAppsButton* chatIcon;
	PhoneExitButton* exitButton;

	static Phone* _instance;
};