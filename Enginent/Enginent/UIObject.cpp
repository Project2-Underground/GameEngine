#include "UIObject.h"
#include "Game.h"
#include "SquareMeshVbo.h"


UIObject::UIObject() {
}

UIObject::~UIObject() {
}

void UIObject::SetTexture(string path) {
	texture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

unsigned int UIObject::GetTexture() {
	return texture;
}

void UIObject::Render() {
	SquareMeshVbo* squareMesh = dynamic_cast<SquareMeshVbo*> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));

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
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(modeId, 1);
		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->Render();

	}
}