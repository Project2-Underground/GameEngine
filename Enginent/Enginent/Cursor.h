#pragma once
#include "InteractObj.h"
#include "UIObject.h"

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
		unsigned int CS_Puzzle;
		bool enable;

	public:
		CursorUI();
		~CursorUI();
		void updateCursor();
		void setCursor(InteractTypeList);
		void enableChange(bool);
		void ResetCursor() { SetTexture(CS_Normal); }
};