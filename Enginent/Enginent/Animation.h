#pragma once
#include <vector>
#include <map>
#include <string>

#include "ImageObject.h"

class Animation {
	std::string animationName;
	std::map<std::string ,Animation> nextAnimation;
	int frame;				// assume only one row of animations
	int fps;
	ImageObject object;
	// int x_interval;
	// int y_interval;
public:
	void AddNextAnimation(std::string, Animation animation);
	void setFrame(int frameNum);
	void setFPS(int newFPS);
	void Update();
	/*void setXInterval(int newX_interval);
	void setYInterval(int newY_interval);*/
};