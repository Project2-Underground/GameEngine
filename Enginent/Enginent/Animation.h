#pragma once
#include <vector>
#include <map>
#include <string>
#include <GL\glew.h>

class Animation {
private:
	float frameWidth = 0.0f;
	int frame = 0;								// assume only changing the x value
	int currentFrame = 0;
	double time = 0.0;
	double period = 0.0;
	std::string texturePath;
public:
	bool changed = false;
	std::string animationName;
	Animation(std::string name, std::string TexturePath);
	bool Finished();
	void SetFrame(int frame);
	void SetFramePeriod(double time);
	std::string GetTexture();						// set animation texture to the ref object
	void GetCurrentFrame(GLfloat* texData);			// calculates and return the new texData[] 
	void NextFrame();
	void ResetAnimation();
	bool ChangeFrame();
};