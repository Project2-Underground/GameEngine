#pragma once

#include "UIObject.h"
#include "Button.h"

enum AppType {
	NOTE = 0,
	CHAT
};

struct Note {
	std::string name;
	unsigned int texture;
	Note(std::string n, unsigned int text);
};

class Chat{
	UIObject* pic;						// profile picture (?)
	std::vector<std::string> texts;		// store all messages from the chat
	// bubble message
public:
	std::string name;					// name of the chat
	int currentMsgIndex;				// show up to this message

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

	void LeftClick(float, float);
	void Next();
	void Back();
	void AddNote(UIObject*);
	void AddChat(std::string);
	void Clear();

	bool open;
	AppType currentAppType;
	int currentPage;

	std::vector<Note*> notes;
	std::vector<Chat*> chats;
	std::vector<UIObject*> buttons;

	~Application();
private:
	UIObject* currentNote;
	UIObject* appBG;
	PhoneNextButton* next;
	PhoneBackButton* back;
	PhoneHomeButton* home;
};

class Phone{
public:
	static Phone* GetInstance();

	void Render();
	void LeftClick(float, float);
	void UpdateButton(float, float);

	void OpenApp(AppType);
	void CloseApp();
	void AddPage(AppType, std::string);
	void SetNotification(AppType);
	void Open();
	void Close();

	bool open;
	Application* app;

	~Phone();

	std::map<std::string, UIObject*> notes;
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