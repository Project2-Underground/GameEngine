#pragma once

#include <map>

#include "Animation.h"
#include "DrawableObject.h"

class Animator {
private:
	DrawableObject* ref;
	std::map<std::string, Animation*> animations;
	Animation* currentAnimation;
	bool loop;
public:
	void AddAnimation(Animation* animation, DrawableObject* ref);
	void Play(std::string animationName, bool loop);
	void Update();
	void ResetSquareMeshVbo();
};