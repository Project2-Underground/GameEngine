#pragma once

#include "UIObject.h"
#include "Button.h"

constexpr auto SCROLL_SPEED = 20;

constexpr auto TEXT_SPACE = 30;
constexpr auto TAB_SPACE = 80;
constexpr auto TAB_SIZE_X = 200.0f;
constexpr auto TAB_TEXT_PADDING = 20.0f;
constexpr auto TAB_TOP_Y = 175;
constexpr auto TEXT_TOP_Y = 100;
constexpr auto TEXT_BOTTOM_Y = -150.0f;
constexpr auto TEXT_START_X = -300.0f;
constexpr auto FONT_SIZE = 24;
constexpr auto MAX_FONT_PER_LINE = 45;

enum AppType {
	NOTE = 0,
	CHAT
};

struct NoteInfo {
	std::string name;
	std::vector<std::string> texts;
	std::vector<glm::vec3> textPosition;
	int lineCount;
	bool noti = true;
	void AddText(std::string);
};

struct ChatInfo {
	unsigned int picTexture;
	std::string name;
	std::vector<std::string> texts;
	std::vector<glm::vec3> textPosition;
	std::vector<int> lineCounts;
	int currentMsgIndex = 0;
	bool noti = true;

	void AddText(std::string);
};

class Note {
	SDL_Color textColor = { 255, 255, 255, 255 };
	TextObject* name;
	UIObject* textBubble;
	UIObject* scrollBar;
	UIObject* thumb;
	std::vector<TextObject*> allMsg;
	float upperBound;
	float lowerBound;
	float scrollBarSpeed;
public:
	Note();
	~Note();
	void ClearText();
	void OpenNote(const NoteInfo);
	void Scroll(int direction);
	void CloseNote();
	void Render();
};

class Chat{
	SDL_Color textColor = { 0, 0, 0, 0 };
	UIObject* profilePic;
	TextObject* name;
	UIObject* textBubble;
	UIObject* scrollBar;
	UIObject* thumb;
	std::vector<TextObject*> allMsg;
	float upperBound;
	float lowerBound;
	float scrollBarSpeed;
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
	Application();

	void Render();
	void LeftClick(float, float);
	void LeftRelease(float, float);
	void Scroll(int direction);
	void Back();
	void AddNote(NoteInfo*);
	void AddChat(ChatInfo*);
	void SetNote(NoteInfo*);
	void SetChat(ChatInfo*);
	void OpenChat();
	void OpenNote();
	void SelectItem(int);
	void Clear();
	bool Unread(AppType);
	bool HasItem(AppType, std::string);

	void Open(AppType);
	void Close();

	bool open;
	bool openNoteChat;
	AppType currentAppType;
	int currentPage;

	std::vector<ChatNoteInfoButton*> itemTabs;
	std::vector<NoteInfo*> notes;
	std::vector<ChatInfo*> chats;
	std::vector<UIObject*> buttons;

	~Application();
private:
	UIObject* notifyPopup;
	Chat* currentChat;
	Note* currentNote;
	UIObject* appBG;
	// PhoneNextButton* next;
	PhoneBackButton* back;
	PhoneHomeButton* home;

	float upperBound;
	float lowerBound;
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
	void SetPage(AppType, std::string);
	void Message(std::string, int);
	void Open();
	void Close();
	void Scroll(glm::vec3, int);
	void PrintAllChat();
	void Clear();
	void AddNPCInteracted(std::string name);

	bool open;
	bool notiNote;
	bool notiChat;
	bool firstClose;
	bool note23Done;
	Application* app;

	~Phone();

	std::map<std::string, NoteInfo> notes;
	std::map<std::string, ChatInfo> chats;
	std::vector<std::string> npcTalked;
	std::string textAfterClose;
private:

	Phone();
	std::vector<UIObject*> icons;
	UIObject* notifyPopup;

	UIObject* phone;
	PhoneAppsButton* noteIcon;
	PhoneAppsButton* chatIcon;
	PhoneExitButton* exitButton;

	static Phone* _instance;
};