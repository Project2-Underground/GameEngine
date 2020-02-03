#include "MouseInput.h"

MouseInput::MouseInput()
{

}

MouseInput::~MouseInput()
{
	if(_instance != nullptr)
	delete _instance;
}

MouseInput* MouseInput::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new MouseInput();
	}
	return _instance;
}

glm::vec3 MouseInput::FindMousePosition(float x, float y)
{
	float realX, realY;
	realX = -(winWidth * 0.5) + x;
	realY = -(winHeight * 0.5) + (winHeight - y);
	return glm::vec3(realX, realY, 1);
}

void MouseInput::UpdateMouseInput(int type, float posX, float posY)
{
	this->position = FindMousePosition(posX, posY);
	this->eventType = type;
}