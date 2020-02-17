#pragma once    

#include "Button.h"

class ViewWindow {
	bool display;
	bool trigger;
	static ViewWindow* instance;
	UIObject* viewItem;
	UIObject* viewWindow;
	ViewWindowClose* closeButton;
	// textbox to display description
public:
	static ViewWindow* GetInstance();

	ViewWindow();
	~ViewWindow();

	void Update();
	void Init(int width, int height);
	void Render();
	void LeftClick(float, float);

	void SetViewItem(unsigned int texture);
	void SetText();
	void Close();
	void Open();
	bool IsOpen() { return display; }
};