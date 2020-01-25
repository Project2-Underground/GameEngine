#pragma once    

#include "Button.h"

class ViewWindow : public UIObject {
	static ViewWindow* instance;
	UIObject* viewItem;
	ViewWindowClose* closeButton;
	// textbox to display description
public:
	static ViewWindow* GetInstance();

	ViewWindow();

	void SetViewItem(unsigned int texture);
	void SetText();
};