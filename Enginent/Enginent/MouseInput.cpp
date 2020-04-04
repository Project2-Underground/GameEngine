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
	game = Game::GetInstance();
}

void MouseInput::FindMousePosition(float x, float y)
{
	float realX, realY;
	realX = -((float)winWidth * 0.5f) + x;
	realY = -((float)winHeight * 0.5f) + ((float)winHeight - y);

	position_Screen = glm::vec3(realX, realY, 1.0f);
	position_World = glm::vec3(realX - Game::GetInstance()->GetCamera()->GetPosition().x, realY - Game::GetInstance()->GetCamera()->GetPosition().y, 1);

}

void MouseInput::UpdateMouseInput(MouseEvent type, int posX, int posY, int direction)
{
	if (!Game::GetInstance()->isLoading) {
		FindMousePosition((float)posX, (float)posY);
		switch (type)
		{
		case RightClick:
		{
			game->GetScreen()->RightClick(position_Screen, position_World);
			ResetActionType();
			break;
		}
		case LeftClick:
		{
			game->GetScreen()->LeftClick(position_Screen, position_World);
			break;
		}
		case Hover:
		{
			game->GetScreen()->UpdateMouseState(position_Screen, position_World);
			break;
		}
		case RightRelease:
		{
			game->GetScreen()->RightRelease(position_Screen, position_World);
			break;
		}
		case LeftRelease:
		{
			game->GetScreen()->LeftRelease(position_Screen, position_World);
			break;
		}
		case Scroll:
		{
			game->GetScreen()->Scroll(position_Screen, direction);
			break;
		}
		}
	}
}
