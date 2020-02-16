#include "Animator.h"
#include "Game.h"
#include "SquareMeshVbo.h"

#include <iostream>

void Animator::Play(std::string animationName, bool loop) {
	// check if animation is added in map
	if (currentAnimation && currentAnimation->hasLeaveTime && !currentAnimation->Finished()) {
		return;
	}
	else {
		if (animations.find(animationName) == animations.end()) {
			std::cout << "Animation name is not found" << std::endl;
			return;
		}
		currentAnimation = animations[animationName];
		this->loop = loop;
		currentAnimation->ResetAnimation();
		currentAnimation->NextFrame();
	}
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
	if (currentAnimation->ChangeFrame())
		currentAnimation->NextFrame();
	if (currentAnimation->Finished() && !loop) 
		Play(defaultAnimation, true);
}


void Animator::AddAnimation(Animation* animation) {
	animations[animation->animationName] = animation;
}

bool Animator::IsPlaying(std::string name) {
	if (!currentAnimation)
		return false;
	return (currentAnimation->animationName == name);
}

Animator::~Animator() {
	std::map<std::string, Animation*>::iterator it;
	for (it = animations.begin(); it != animations.end(); ++it)
		delete it->second;
}