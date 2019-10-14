
#include "ImageObject.h"
#include "Game.h"
#include "SquareMeshVbo.h"


ImageObject::ImageObject()
{
	col = nullptr;
	anim = nullptr;
}


ImageObject::~ImageObject()
{
}

void ImageObject::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
}

unsigned int ImageObject::GetTexture() {
	return texture;
}

void ImageObject::SetTexture(string path)
{
	texturePath = path;
	texture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

void ImageObject::Render(glm::mat4 globalModelTransform)
{
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));

	if (anim != nullptr)
		anim->Update();

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
		squareMesh->ResetTexData();
	}
}

