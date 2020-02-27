#pragma once
#include "Game.h"

enum MouseEvent{
	RightClick = 0,
	LeftClick,
	Hover,
	RightRelease,
	LeftRelease,
	Scroll
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
	int winWidth, winHeight;
	Game* game;
protected:
	MouseInput();
public:
	~MouseInput();
	static MouseInput* GetInstance();
	void Init(int width, int height);
	void FindMousePosition(float x, float y);
	void UpdateMouseInput(MouseEvent, int, int, int direction = 0);
	void SetActionEventType(ActionEvent a) { actionType = a; }
	glm::vec3 GetMouseScreenPosition() { return position_Screen; }
	glm::vec3 GetMouseWorldPosition() { return position_World; }
	int GetEvent();
	void ResetActionType() { actionType = NORMAL_ACTION; }
	ActionEvent GetActionEvent() { return actionType; }
	
};