#include "Animator.h"
#include "Game.h"
#include "SquareMeshVbo.h"
#include "ImageObject.h"

void Animator::AddAnimation(Animation* animation, DrawableObject* ref) {
	this->ref = ref;
	animations[animation->GetName()] = animation;
	currentAnimation = animation;
}

void Animator::Play(std::string animationName, bool loop) {
	this->loop = loop;
	currentAnimation = animations[animationName];
	((ImageObject*)ref)->SetTexture(currentAnimation->GetTexturePath());
}

void Animator::Update() {
	if (loop || !currentAnimation->Finished()) {
		SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));
		if(squareMesh != nullptr)
			squareMesh->setNewTexData(currentAnimation->GetNextFrame());
	}
}

void Animator::ResetSquareMeshVbo() {
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));
	if (squareMesh != nullptr)
		squareMesh->ResetTexData();
}