#pragma once
#include <vector>
#include <map>
#include <string>
#include <GL\glew.h>
#include "ImageObject.h"

class Animation {
	ImageObject* ref;
	int frame;									// assume only one row of animations
	float currentX;
	int x_interval;
	// int y_interval;
public:
	std::string animationName;
	int fps;

	Animation(ImageObject* object, std::string name);
	void SetFrame(int frameNum);
	void SetFPS(int newFPS);
	GLfloat* GetNextFrame();
	void SetXInterval(int newX_interval);
	void ResetAnimation();
	// void setYInterval(int newY_interval);
};