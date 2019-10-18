#include "Animator.h"
#include "Game.h"
#include "SquareMeshVbo.h"

#include <iostream>

void Animator::Play(std::string animationName, bool loop) {
	// check if animation is added in map
	if(animations.find(animationName) == animations.end()){
		std::cout << "Animation name is not found" << std::endl;
		return;
	}

	currentAnimation->ResetAnimation();
	currentAnimation = animations[animationName];
	currentAnimation->SetTexture();
	this->loop = loop;
}

void Animator::SetDefaultAnimation(std::string animationName) {
	// check if animation is added in map
	if (animations.find(animationName) == animations.end()) {
		std::cout << "Animation name is not found" << std::endl;
		return;
	}
	defaultAnimation = animationName;
}

void Animator::Update() {
	if (loop || !currentAnimation->Finished()) {
		SquareMeshVbo* squareMesh = dynamic_cast<SquareMeshVbo*> (Game::GetInstance()->GetRenderer()->GetMesh(SquareMeshVbo::MESH_NAME));
		squareMesh->setNewTexData(currentAnimation->GetNextFrame());
		if (currentAnimation->Finished() && !loop) {
			Play(defaultAnimation, true);
		}
	}
}