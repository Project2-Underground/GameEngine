#include "MouseInput.h"
#include "Game.h"

MouseInput* MouseInput::_instance = nullptr;

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

void MouseInput::Init(int width, int height)
{
	this->winHeight = height;
	this->winWidth = width;
}

glm::vec3 MouseInput::FindMousePosition(float x, float y)
{
	float realX, realY; 
	realX = -(winWidth * 0.5f) + x - Game::GetInstance()->GetCamera()->GetPosition().x;
	realY = -(winHeight * 0.5f) + (winHeight - y) - Game::GetInstance()->GetCamera()->GetPosition().y;
	return glm::vec3(realX, realY, 1);
}

void MouseInput::UpdateMouseInput(int type, float posX, float posY)
{
	this->position = FindMousePosition(posX, posY);
	this->eventType = type;
}