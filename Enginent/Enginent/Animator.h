#pragma once

#include <map>

#include "Animation.h"

class Animator {
private:
	std::map<std::string, Animation*> animations;
	Animation* currentAnimation;
	bool stop;
public:
	void Play(std::string animationName, bool loop);
};