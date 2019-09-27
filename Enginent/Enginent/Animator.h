#pragma once
#include <map>
#include <string>

#include "Animation.h"

class Animator {
	std::map<std::string, Animation> animationList;
	std::map<std::string, bool> parameterList;
	std::string currentPlayingName;
public:
	void addAnimation(Animation animation);
	void setAnimation(std::string animationToSet, std::string animationFrom, std::string paraName, bool b);	// works similar to unity's animation
	void setBool(std::string paraName, bool b);
	void Update();
};