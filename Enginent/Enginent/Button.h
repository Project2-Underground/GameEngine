#pragma once
#include "InteractObj.h"
#include "UIObject.h"
#include <iostream>

class Button : public UIObject {
	protected:
		ImageObject* normalTexture;
		ImageObject* hoverTexture;
		ImageObject* pressTexture;
	public:
		Button(std::string, std::string, std::string);
		void updateButton(int, int);
		virtual void action(int x, int y);
		void checkCollider(int x, int y);
};

class Exit_Button : public Button {
	public:
		~Exit_Button();
		Exit_Button(std::string normal, ::string hover, std::string press) : Button(normal, hover, press) {};
		void action(int x, int y);
};

class SwitchScene_Button : public Button {
public:
	SwitchScene_Button(std::string normal, std::string hover, std::string press) : Button(normal, hover, press) {};
	~SwitchScene_Button();
	void action(int x, int y);
};