#pragma once
#include <vector>
#include <map>
#include <string>
#include <GL\glew.h>

class Animation {
private:
	std::string texture;
	float frameWidth;
	int frame;								// assume only changing the x value
	int currentFrame;
	std::string animationName;
public:
	Animation(std::string name, std::string path);

	void SetFrame(int frame);
	void ResetAnimation();

	GLfloat* GetNextFrame();				// calculates and return the new texData[] 
	bool Finished();
	std::string GetName();
	std::string GetTexturePath();
};