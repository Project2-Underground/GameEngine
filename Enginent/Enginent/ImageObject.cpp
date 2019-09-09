
#include "ImageObject.h"
#include "Game.h"
#include "SquareMeshVbo.h"


ImageObject::ImageObject()
{

}


ImageObject::~ImageObject()
{
}

void ImageObject::SetTexture(string path)
{
	texture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

void ImageObject::Render(glm::mat4 globalModelTransform)
{
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = Game::GetInstance()->GetRenderer()->GetModelMatrixAttrId();
	GLuint modeId = Game::GetInstance()->GetRenderer()->GetModeUniformId();

	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (modeId == -1) {
		cout << "Error: Can't set mode in ImageObject " << endl;
		return;
	}

	vector <glm::mat4> matrixStack;

	glm::mat4 currentMatrix = this->getTransform();

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(modeId, 1);
		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->Render();

	}
}

