#pragma once
#include "ImageObject.h"
#include "UIObject.h"

class CursorUI : public UIObject {
	private:
		ImageObject* CS_Normal;
		ImageObject* CS_Chat;
		ImageObject* CS_Pick;
		ImageObject* CS_Door;
		ImageObject* CS_View;
		ImageObject* CS_Save;

	public:
		CursorUI();
		~CursorUI();
		void updateCursor();
		void setCursor(int type);
};