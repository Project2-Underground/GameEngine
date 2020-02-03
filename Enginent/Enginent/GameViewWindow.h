#pragma once    

#include "Button.h"

class ViewWindow {
	bool display;
	static ViewWindow* instance;
	UIObject* viewItem;
	UIObject* viewWindow;
	ViewWindowClose* closeButton;
	// textbox to display description
public:
	static ViewWindow* GetInstance();

	ViewWindow();
	~ViewWindow();

	void Init(int width, int height);
	void Render();
	void LeftClick(int, int);

	void SetViewItem(unsigned int texture);
	void SetText();
	void Close();
	void Open();
	bool IsOpen() { return display; }
};