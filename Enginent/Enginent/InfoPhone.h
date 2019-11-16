#pragma once

#include "UIObject.h"

class InfoPhone : public UIObject {
	UIObject* noteIcon;
	UIObject* chatIcon;
	UIObject* exitButton;
public:
	InfoPhone();
	void Open();			// bring up the Phone onto the screen
	void Chat();
	void Note();
};