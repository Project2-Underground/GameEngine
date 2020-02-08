#pragma once
#include "Game.h"

enum MouseEvent{
	RightClick = 0,
	LeftClick,
	Hover,
	Drag
};

enum ActionEvent {
	NORMAL_ACTION = 0,
	SEPARATE_ACTION,
	COMBINE_ACTION,
	ITEM_SELECTED_ACTION
};

class MouseInput {
	static MouseInput* _instance;
	glm::vec3 position_Screen;
	glm::vec3 position_World;
	MouseEvent eventType;
	ActionEvent actionType;
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
	void UpdateMouseInput(MouseEvent, int, int);
	void SetActionEventType(ActionEvent a) { actionType = a; }
	int GetEvent();
	void ResetActionType() { actionType = NORMAL_ACTION; }
	ActionEvent GetActionEvent() { return actionType; }
	
};