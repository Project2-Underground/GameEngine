#include "Cursor.h"
#include "Game.h"

CursorUI::CursorUI() {
	CS_Normal = new ImageObject();
	CS_Normal->SetTexture("Texture/UI/Normal_Cursor.png");
	CS_Chat = new ImageObject();
	CS_Chat->SetTexture("Texture/UI/Chat_cursor.png");
	CS_Door = new ImageObject();
	CS_Door->SetTexture("Texture/UI/Move_cursor.png");
	CS_Pick = new ImageObject();
	CS_Pick->SetTexture("Texture/UI/Pickup_cursor.png");
	CS_View = new ImageObject();
	CS_View->SetTexture("Texture/UI/View_cursor.png");
	CS_Save = new ImageObject();
	CS_Save->SetTexture("Texture/UI/Save_cursor.png");


	this->SetTexture(((CursorUI*)CS_Normal)->texture);
	this->SetSize(25, -25);
	this->SetPosition(glm::vec3(0, 0, 1));
}

CursorUI::~CursorUI() {
	delete CS_Normal;
	delete CS_Chat;
	delete CS_Pick;
	delete CS_Door;
	delete CS_View;
	delete CS_Save;
}

void CursorUI::updateCursor()
{
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	glm::vec3 realPos = Game::GetInstance()->FindMousePosition(mouseX + (this->getSize().x / 2), mouseY - (this->getSize().y / 2));
	this->SetPosition(realPos);
	setCursor(Game::GetInstance()->checkPointObject(realPos));
}

void CursorUI::setCursor(int type)
{
	switch (type)
	{
		case IneractTypeList::NORMAL:
				this->SetTexture(((CursorUI*)CS_View)->texture);
				break;
		case IneractTypeList::TALK:
				this->SetTexture(((CursorUI*)CS_Chat)->texture);
				break;
		case IneractTypeList::CHANGESCENE:
				this->SetTexture(((CursorUI*)CS_Door)->texture);
				break;
		case IneractTypeList::PICKUP:
				this->SetTexture(((CursorUI*)CS_Pick)->texture);
				break;
		case IneractTypeList::VIEW:
				this->SetTexture(((CursorUI*)CS_View)->texture);
				break;
		case IneractTypeList::SAVE:
				this->SetTexture(((CursorUI*)CS_Save)->texture);
				break;
		default:
				this->SetTexture(((CursorUI*)CS_Normal)->texture);
				break;
	}
}