#pragma once
#include "Game.h"

int winWidth = 1280;
int winHeight = 720;

enum MouseEvent{
	RightClick = 0,
	LeftClick,
	Hover,
	Drag
};

class MouseInput {
	MouseInput* _instance = nullptr;
	glm::vec3 position;
	int eventType;
	bool trigger = false;
protected:
	MouseInput();
public:
	~MouseInput();
	MouseInput* GetInstance();
	glm::vec3 FindMousePosition(float x, float y);
	void UpdateMouseInput(int, float, float);

};