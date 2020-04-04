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
	void SetDefaultAnimation(std::string animationName);		
	void Play(std::string animationName, bool loop);
	void Play(bool loop);										// plays default animation
	void Update();
	void AddAnimation(Animation* animation);
	void AddAnimation(std::string name, std::string texture, int frameNo, float frameRate, bool loop = false);
	void AddAnimation(std::string name, unsigned int  texture, int frameNo, float frameRate, bool loop = false);
	~Animator();
};