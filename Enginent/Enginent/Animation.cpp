#include "Animation.h"
#include "Game.h"

Animation::Animation(std::string name, std::string path) {
	texture = path;
	this->animationName = name;
}

std::string Animation::GetTexturePath() {
	return texture;
}


std::string Animation::GetName() {
	return animationName;
}

void Animation::SetFrame(int frame) {
	this->frame = frame;
	frameWidth = 1.0f / (float)frame;
	currentFrame = 0;
}

GLfloat* Animation::GetNextFrame() {
	currentFrame %= frame;
	float tmp = currentFrame * frameWidth;
	GLfloat newTexData[] =
	{
	  0.0f + tmp, 0.0f,
	  frameWidth + tmp, 0.0f,
	  frameWidth + tmp, 1.0f,
	  0.0f + tmp, 1.0f
	};

	currentFrame++;

	return newTexData;
}

bool Animation::Finished() {
	if (currentFrame == frame) 
		return true;
	return false;
}

void Animation::ResetAnimation() {
	currentFrame = 0;
}