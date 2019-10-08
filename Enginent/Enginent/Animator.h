#pragma once
#include <map>
#include <string>

#include "Animation.h"

class Animator {
	std::map<std::string, Animation*> animationList;
	std::string currentPlayingName;
	//std::map<std::string, bool> parameterList;
public:
	Animation* currentAnim;
	void AddAnimation(Animation* animation);
	void PlayAnimation(std::string name);
	std::string GetCurrentPlayingName();
	//void Update();
	//void setAnimation(std::string animationToSet, std::string animationFrom, std::string paraName, bool b);	// works similar to unity's animation
	//void setBool(std::string paraName, bool b);
};