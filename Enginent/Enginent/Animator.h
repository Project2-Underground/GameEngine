#pragma once

#include <map>

#include "Animation.h"

class Animator {
private:
	std::map<std::string, Animation*> animations;
	std::string defaultAnimation;
	bool loop;
public:
	Animation* currentAnimation;
	void SetDefaultAnimation(std::string animationName);		// usually idle animation
	void Play(std::string animationName, bool loop);
	void Update();
	void AddAnimation(Animation* animation);
	~Animator();
};