#include "Cursor.h"
#include "Game.h"
#include "MouseInput.h"

CursorUI::CursorUI() {
	GLRenderer *renderer = Game::GetInstance()->GetRenderer();
	CS_Normal = renderer->LoadTexture("Texture/UI/Cursor/Normal_Cursor.png");
	CS_Chat = renderer->LoadTexture("Texture/UI/Cursor/Chat_cursor.png");
	CS_Door = renderer->LoadTexture("Texture/UI/Cursor/Move_cursor.png");
	CS_Pick = renderer->LoadTexture("Texture/UI/Cursor/Pickup_cursor.png");
	CS_View = renderer->LoadTexture("Texture/UI/Cursor/View_cursor.png");
	CS_Save = renderer->LoadTexture("Texture/UI/Cursor/Save_cursor.png");
	CS_Stair = renderer->LoadTexture("Texture/tmp_texture/tmp_stairsSign.png");
	CS_Note = renderer->LoadTexture("Texture/UI/InfoPhone/Note_app.png");
	enable = true;

	this->SetTexture(CS_Normal);
	this->SetSize(40.0f, -40.0f);
	this->SetPosition(glm::vec3(0, 0, 1));
}

CursorUI::~CursorUI() {

}

void CursorUI::updateCursor()
{
	Game* game = Game::GetInstance();
	MouseInput* m = MouseInput::GetInstance();
	//int mouseX, mouseY;
	//SDL_GetMouseState(&mouseX, &mouseY);
	//glm::vec3 realPos = game->FindMousePosition((int)(mouseX + (this->getSize().x / 2)), (int)(mouseY - (this->getSize().y / 2)));
	glm::vec3 mousePosition = m->GetMouseScreenPosition();
	this->SetPosition(mousePosition);
	//std::cout << "Update Cursor\n";
	glm::vec3 mouseSpacePosition = m->GetMouseWorldPosition();
	setCursor(game->GetScreen()->GetPointedObject(mouseSpacePosition));
}

void CursorUI::setCursor(InteractTypeList type)
{
	if (enable)
	{
		//std::cout << "in\n";
		//std::cout << type << std::endl;
		switch (type)
		{
			case InteractTypeList::NORMAL:
				this->SetTexture(CS_Normal);
				break;
			case InteractTypeList::TALK:
				this->SetTexture(CS_Chat);
				break;
			case InteractTypeList::DOOR:
				this->SetTexture(CS_Door);
				break;
			case InteractTypeList::PICKUP:
				this->SetTexture(CS_Pick);
				break;
			case InteractTypeList::VIEW:
				this->SetTexture(CS_View);
				break;
			case InteractTypeList::SAVE:
				this->SetTexture(CS_Save);
				break;
			case InteractTypeList::STAIR:
				this->SetTexture(CS_Stair);
				break;
			case InteractTypeList::ADDNOTE:
				this->SetTexture(CS_Note);
				break;
			case InteractTypeList::PUZZLE:
				this->SetTexture(CS_Pick);
				break;
			default:
				this->SetTexture(CS_Normal);
				break;
		}
	}
	else {
		this->SetTexture(CS_Normal);
	}
}
void CursorUI::EnableCursor(unsigned char c, bool b) {
	if (b) 
		cursorDisableCheck |= c;
	else
		cursorDisableCheck &= ~c;
	enable = !(cursorDisableCheck & 255);
}
void CursorUI::ResetCursor() {
	cursorDisableCheck &= 0;
	enable = true;
	SetTexture(CS_Normal);
}
bool CursorUI::IsEnable() {
	return enable;
}