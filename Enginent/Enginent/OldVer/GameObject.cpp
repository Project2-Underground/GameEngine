
#include "GameObject.h"
#include "Game.h"
#include "SquareMeshVbo.h"


GameObject::GameObject()
{
	color = glm::vec3(0.0, 0.0, 0.0);
}


GameObject::~GameObject()
{
}

void GameObject::SetColor(float r, float g, float b)
{
	color = glm::vec3(r, g, b);
}

void GameObject::Render(glm::mat4 globalModelTransform)
{
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = Game::GetInstance()->GetRenderer()->GetModelMatrixAttrId();
	GLuint colorId = Game::GetInstance()->GetRenderer()->GetColorUniformId();


	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (colorId == -1) {
		cout << "Error: Can't set color " << endl;
		return;
	}
	vector <glm::mat4> matrixStack;

	glm::mat4 currentMatrix = this->getTransform();

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, color.x, color.y, color.z);
		squareMesh->Render();

	}
}

