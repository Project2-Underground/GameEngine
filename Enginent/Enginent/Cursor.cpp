#include "Cursor.h"
#include "Game.h"

CursorUI::CursorUI() {
	CS_Normal = new ImageObject();
	CS_Normal->SetTexture("Texture/UI/Cursor/Normal_Cursor.png");
	CS_Chat = new ImageObject();
	CS_Chat->SetTexture("Texture/UI/Cursor/Chat_cursor.png");
	CS_Door = new ImageObject();
	CS_Door->SetTexture("Texture/UI/Cursor/Move_cursor.png");
	CS_Pick = new ImageObject();
	CS_Pick->SetTexture("Texture/UI/Cursor/Pickup_cursor.png");
	CS_View = new ImageObject();
	CS_View->SetTexture("Texture/UI/Cursor/View_cursor.png");
	CS_Save = new ImageObject();
	CS_Save->SetTexture("Texture/UI/Cursor/Save_cursor.png");

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
	Game* game = Game::GetInstance();
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	glm::vec3 realPos = game->FindMousePosition((int)(mouseX + (this->getSize().x / 2)), (int)(mouseY - (this->getSize().y / 2)));
	this->SetPosition(realPos);
	setCursor(game->GetScreen()->GetPointedObject(realPos));
}

void CursorUI::setCursor(int type)
{
	if (enable)
	{
		switch (type)
		{
			case InteractTypeList::NORMAL:
				this->SetTexture(((CursorUI*)CS_View)->texture);
				break;
			case InteractTypeList::TALK:
				this->SetTexture(((CursorUI*)CS_Chat)->texture);
				break;
			case InteractTypeList::CHANGESCENE:
				this->SetTexture(((CursorUI*)CS_Door)->texture);
				break;
			case InteractTypeList::PICKUP:
				this->SetTexture(((CursorUI*)CS_Pick)->texture);
				break;
			case InteractTypeList::VIEW:
				this->SetTexture(((CursorUI*)CS_View)->texture);
				break;
			case InteractTypeList::SAVE:
				this->SetTexture(((CursorUI*)CS_Save)->texture);
				break;
			default:
				this->SetTexture(((CursorUI*)CS_Normal)->texture);
				break;
		}
	}

}

void CursorUI::enableChange(bool e)
{
	enable = e;
}