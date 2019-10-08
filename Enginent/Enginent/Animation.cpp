#include "Animation.h"

Animation::Animation(ImageObject* object, std::string name) {
	ref = object;
	animationName = name;
	currentX = 0;
}

void Animation::SetFrame(int frameNum) {
	this->frame = frameNum;
}

void Animation::SetFPS(int newFPS) {
	this->fps = newFPS;
}

GLfloat* Animation::GetNextFrame() {
	currentX += x_interval;
	if (currentX >= x_interval * frame) {
		currentX = 0;
	}

	GLfloat texData[] =
	{
	  0.0f + currentX, 0.0f,
	  1.0f + currentX, 0.0f,
	  1.0f + currentX, 1.0f,
	  0.0f + currentX, 1.0f
	};

	return texData;
}

void Animation::SetXInterval(int newX_interval) {
	this->x_interval = newX_interval;
}


void Animation::ResetAnimation() {
	currentX = 0;
}

//void Animation::setYInterval(int newY_interval) {
//
//}