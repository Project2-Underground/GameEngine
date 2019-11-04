#include "Cursor.h"
#include "Game.h"

CursorUI::CursorUI() {
	normalCS = new ImageObject();
	normalCS->SetTexture("Texture/UI/normal_cursor.png");
	this->SetTexture(((CursorUI*)normalCS)->texture);
	this->SetSize(25, -25);
	this->SetPosition(glm::vec3(0, 0, 1));
}

CursorUI::~CursorUI() {
	delete normalCS;
	//delete chatCS;
}

void CursorUI::updateCursor()
{
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	this->SetPosition(Game::GetInstance()->findRealPos(mouseX + (this->getSize().x/2),mouseY - (this->getSize().y/2)));
	//if (SDL_MOUSEMOTION)
	//{

	//}
}
void CursorUI::setCursor()
{

}