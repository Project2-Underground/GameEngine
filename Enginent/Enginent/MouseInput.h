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
	glm::vec3 position_Screen;
	glm::vec3 position_World;
	int eventType;
	bool trigger = false;
	int winWidth, winHeight;
	Game* game;
protected:
	MouseInput();
public:
	~MouseInput();
	static MouseInput* GetInstance();
	void Init(int width, int height);
	void FindMousePosition(float x, float y);
	void UpdateMouseInput(int, float, float);
	int GetEvent();
	void ClearInput();

};