#pragma once
#include <vector>
#include <map>
#include <string>
#include <GL\glew.h>
#include "ImageObject.h"

class Animation {
private:
	ImageObject* ref;
	float frameWidth;
	int frame;								// assume only changing the x value
	int currentFrame;
	std::string animationName;
public:
	Animation(ImageObject* ref, std::string name);
	bool Finished();
	void SetFrame(int frame);
	GLfloat* GetNextFrame();				// calculates and return the new texData[] 
	void ResetAnimation();
};