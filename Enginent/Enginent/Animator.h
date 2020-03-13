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
	bool IsPlaying(std::string);
	void SetDefaultAnimation(std::string animationName);		// usually idle animation
	void Play(std::string animationName, bool loop);
	void Update();
	void AddAnimation(Animation* animation);
	void AddAnimation(std::string name, std::string texture, int frameNo, float frameRate, bool loop = false);
	~Animator();
};