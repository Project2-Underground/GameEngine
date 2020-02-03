#pragma once
#include "Game.h"

enum MouseEvent{
	RightClick = 0,
	LeftClick,
	Hover,
	Drag
};

class MouseInput {
	static MouseInput* _instance;
	glm::vec3 position;
	int eventType;
	bool trigger = false;
	int winWidth, winHeight;
protected:
	MouseInput();
public:
	~MouseInput();
	static MouseInput* GetInstance();
	void Init(int width, int height);
	glm::vec3 FindMousePosition(float x, float y);
	void UpdateMouseInput(int, float, float);

};