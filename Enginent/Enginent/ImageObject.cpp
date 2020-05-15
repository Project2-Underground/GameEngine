
#include "ImageObject.h"
#include "Game.h"
#include "SquareMeshVbo.h"


ImageObject::ImageObject()
{
	col = nullptr;
	anim = nullptr;
	display = true;
	texData[0] = 0.0f;
	texData[1] = 0.0f;
	texData[2] = 1.0f;
	texData[3] = 0.0f;
	texData[4] = 1.0f;
	texData[5] = 1.0f;
	texData[6] = 0.0f;
	texData[7] = 1.0f;
}


ImageObject::~ImageObject()
{
	if (col)
		delete col;
	if (anim)
		delete anim;
}

void ImageObject::Init(float sizex, float sizey, glm::vec3 position) {
	SetSize(sizex, sizey);
	SetPosition(position);
	SetCollder(new Collider(this));
}

void ImageObject::SetCollder(Collider* n_col) {
	col = n_col;
	col->setRefObject(this);
}

unsigned int ImageObject::GetTexture() {
	return texture;
}
string ImageObject::GetTexturePath() {
	return texturePath;
}
void ImageObject::Appear(bool b) {
	SetDisplay(b);
	col->enable = b;
}

void ImageObject::SetTexture(string path)
{
	texturePath = path;
	texture = Game::GetInstance()->GetRenderer()->LoadTexture(path);
}

void ImageObject::SetTexturePath(string path) {
	texturePath = path;
}
void ImageObject::SetPosition(glm::vec3 newPosition) {
	DrawableObject::SetPosition(newPosition);
	if (col) 
		col->setNewPos(newPosition);
}

void ImageObject::SetTexture(unsigned int texture) {
	this->texture = texture;
}

void ImageObject::Render(glm::mat4 globalModelTransform)
{
	if (display) {
		SquareMeshVbo* squareMesh = dynamic_cast<SquareMeshVbo*> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));

		if (anim && !anim->currentAnimation->changed) {
			anim->currentAnimation->GetCurrentFrame(texData);
			SetTexture(anim->currentAnimation->GetTexture());
		}
		squareMesh->setNewTexData(texData);
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
}