#pragma once

#include "UIObject.h"
#include "Button.h"

constexpr auto SCROLL_SPEED = 20;

constexpr auto TEXT_SPACE = 30;
constexpr auto TEXT_TOP_Y = 180.0f;
constexpr auto TEXT_BOTTOM_Y = -250.0f;
constexpr auto TEXT_START_X = -150.0f;
constexpr auto FONT_SIZE = 24;
constexpr auto MAX_FONT_PER_LINE = 29;

enum AppType {
	NOTE = 0,
	CHAT
};

struct Note {
	std::string name;
	unsigned int texture;
	Note(std::string n, unsigned int text);
};

struct ChatInfo {
	unsigned int picTexture;
	std::string name;
	std::vector<std::string> texts;
	std::vector<glm::vec3> textPosition;
	std::vector<int> lineCounts;
	int currentMsgIndex = 0;

	void AddText(std::string);
};

class Chat{
	SDL_Color textColor = { 0, 0, 0, 0 };
	UIObject* profilePic;
	TextObject* name;
	std::vector<TextObject*> allMsg;
	float upperBound;
	float lowerBound;
public:				
	~Chat();
	Chat();
	void ClearText();
	void OpenChat(const ChatInfo);
	void Scroll(int direction);
	void CloseChat();
	void Render();
	// OnScroller
};

class Application {
public:
	Application(glm::vec3 phoneSize, glm::vec3 phonePos);
	void Render();

	void LeftClick(float, float);
	void LeftRelease(float, float);
	void Scroll(int direction);
	void Next();
	void Back();
	void AddNote(UIObject*);
	void AddChat(ChatInfo*);
	void OpenChat();
	void Clear();

	bool open;
	AppType currentAppType;
	int currentPage;

	std::vector<Note*> notes;
	std::vector<ChatInfo*> chats;
	std::vector<UIObject*> buttons;

	~Application();
private:
	Chat* currentChat;
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
	void LeftRelease(float, float);
	void LeftClick(float, float);
	void UpdateButton(float, float);

	void OpenApp(AppType);
	void CloseApp();
	void AddPage(AppType, std::string);
	void SetNotification(AppType);
	void ResetNotification(AppType);
	void Message(std::string, int);
	void Open();
	void Close();
	void Scroll(glm::vec3, int);
	void PrintAllChat();
	void Clear();

	bool open;
	bool notiNote;
	bool notiChat;
	Application* app;

	~Phone();

	std::map<std::string, UIObject*> notes;
	std::map<std::string, ChatInfo> chats;
private:
	Phone();
	std::vector<UIObject*> icons;

	UIObject* phone;
	PhoneAppsButton* noteIcon;
	PhoneAppsButton* chatIcon;
	PhoneExitButton* exitButton;

	static Phone* _instance;
};