#include "Cursor.h"
#include "Game.h"

enum cursorType {
	NORMAL = 0,
	CHAT,
	DOOR,
	PICK,
	VIEW,
	SAVE
};

CursorUI::CursorUI() {
	CS_Normal = new ImageObject();
	CS_Normal->SetTexture("Texture/UI/Normal_Cursor_Edit2.png");
	CS_Chat = new ImageObject();
	CS_Chat->SetTexture("Texture/UI/Chat_cursor.png");
	CS_Door = new ImageObject();
	CS_Door->SetTexture("Texture/UI/door_cursor.png");
	CS_Pick = new ImageObject();
	CS_Pick->SetTexture("Texture/UI/pickup_cursor.png");
	CS_View = new ImageObject();
	CS_View->SetTexture("Texture/UI/view_cursor.png");
	CS_Save = new ImageObject();
	CS_Save->SetTexture("Texture/UI/save_cursor.png");


	this->SetTexture(((CursorUI*)CS_Normal)->texture);
	this->SetSize(25, -25);
	this->SetPosition(glm::vec3(0, 0, 1));
}

CursorUI::~CursorUI() {
	delete CS_Normal;
	//delete chatCS;
}

void CursorUI::updateCursor()
{
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	this->SetPosition(Game::GetInstance()->findRealPos(mouseX + (this->getSize().x/2),mouseY - (this->getSize().y/2)));
	setCursor(0);
}
void CursorUI::setCursor(int type)
{
	switch (type)
	{
		case 0:
			this->SetTexture(((CursorUI*)CS_Normal)->texture);
		case 1:
			this->SetTexture(((CursorUI*)CS_Chat)->texture);
		case 2:
			this->SetTexture(((CursorUI*)CS_Door)->texture);
		case 3:
			this->SetTexture(((CursorUI*)CS_Pick)->texture);
		case 4:
			this->SetTexture(((CursorUI*)CS_View)->texture);
		case 5:
			this->SetTexture(((CursorUI*)CS_Save)->texture);

	}
}