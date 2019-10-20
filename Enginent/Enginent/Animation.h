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
	double time;
	double period;
	std::string animationName;
	std::string texturePath;
public:
	Animation(ImageObject* ref, std::string name, std::string TexturePath);
	bool Finished();
	void SetFrame(int frame);
	void SetFramePeriod(double time);
	void SetTexture();						// set animation texture to the ref object
	GLfloat* GetNextFrame();				// calculates and return the new texData[] 
	void ResetAnimation();
	bool ChangeFrame();
};