#pragma once

#include <map>

#include "Animation.h"

class Animator {
private:
	std::map<std::string, Animation*> animations;
	Animation* currentAnimation;
	std::string defaultAnimation;
	bool loop;
public:
	void SetDefaultAnimation(std::string animationName);		// usually idle animation
	void Play(std::string animationName, bool loop);
	void Update();
};