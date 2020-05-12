#pragma once
#include "InteractObj.h"
#include "UIObject.h"

constexpr auto CURSOR_DIALOGUE_ON = 0x01;
constexpr auto CURSOR_PAUSE_ON = 0x02;
constexpr auto CURSOR_OTHER_WINDOW_ON = 0x04;
constexpr auto CURSOR_PUZZLE_ON = 0x08;
constexpr auto CURSOR_PHONE_ON = 0x10;

class CursorUI : public UIObject {
	private:
		unsigned int CS_Normal;
		unsigned int CS_Chat;
		unsigned int CS_Pick;
		unsigned int CS_Door;
		unsigned int CS_View;
		unsigned int CS_Save;
		unsigned int CS_Stair;
		unsigned int CS_Note;

		unsigned char cursorDisableCheck;

		bool enable;

	public:
		CursorUI();
		~CursorUI();
		void updateCursor();
		void setCursor(InteractTypeList);
		void EnableCursor(unsigned char, bool);
		void ResetCursor();
};