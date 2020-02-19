#pragma once    

#include "Button.h"

class GameWindow {
protected:
	bool display;
	bool trigger;
	WindowClose* closeButton;
public:
	virtual void Update() = 0;
	virtual void Init(int width, int height) = 0;
	virtual void Render() = 0;
	virtual void LeftClick(float, float) = 0;

	void Close();
	void Open();
	bool IsOpen() { return display; }
};

class ViewWindow :public GameWindow{
	static ViewWindow* instance;
	UIObject* viewItem;
	UIObject* viewWindow;
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
};

class LoadWindow: public GameWindow {

};