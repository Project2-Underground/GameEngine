#include "pch.h"
#include "TextObject.h"
#include "Game.h"
#include "SquareMeshVbo.h"

TextObject::TextObject()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


TextObject::~TextObject()
{

}

void TextObject::Render()
{
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = Game::GetInstance()->GetRenderer()->GetModelMatrixAttrId();
	GLuint modeId = Game::GetInstance()->GetRenderer()->GetModeUniformId();

	glBindTexture(GL_TEXTURE_2D, texture);
	//glBindTexture(GL_TEXTURE_2D, texNum);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	//glBindTexture(GL_TEXTURE_2D, texture);
	//cout << "Render\n";


	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}

	glm::mat4 currentMatrix = this->getTransform();

	if (squareMesh != nullptr) 
	{
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(modeId, 1);
		//squareMesh->resetTexcoord();
		squareMesh->Render();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void TextObject::update(float deltaTime)
{
}

void TextObject::loadText(string text, SDL_Color textColor, int fontSize)
{
	glBindTexture( GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	TTF_Font * font = TTF_OpenFont("Roboto-Black.ttf", fontSize);
	if (font)
	{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text.c_str(), textColor);
		if (surfaceMessage) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMessage->w, surfaceMessage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surfaceMessage->pixels);
			this->SetSize((float)surfaceMessage->w, (float)-surfaceMessage->h);
			SDL_FreeSurface(surfaceMessage);
		}
		TTF_CloseFont(font);
	}
	else
	{
		cout << "Error: " << TTF_GetError() << endl;
		return;
	}
	
}

unsigned int TextObject::GetTexture()
{
	return this->texture;
}