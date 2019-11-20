#pragma once
#include "InteractObj.h"
#include "UIObject.h"
#include <iostream>

class Button : public UIObject {
	protected:
		ImageObject* hoverTexture;
		ImageObject* pressTexture;
	public:
		Button() {}
		Button(std::string, std::string);
		void pressButton(int x, int y);
		void hoverCursor(int x, int y);
		void updateButton();
		virtual void action(int x, int y);
};

class Exit_Button : public Button {
	public:
		void action(int x, int y);
};

class SwitchScene_Button : public Button {
public:
	SwitchScene_Button() {}
	~SwitchScene_Button();
	SwitchScene_Button(std::string, std::string);
	void action(int x, int y);
};