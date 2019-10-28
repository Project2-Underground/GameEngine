#include "Animation.h"
#include "TimeSystem.h"
#include <iostream>

Animation::Animation( std::string name, std::string TexturePath) {
	this->animationName = name;
	this->texturePath = TexturePath;
}

void Animation::SetFrame(int frame) {
	this->frame = frame;
	frameWidth = 1.0f / (float)frame;
	currentFrame = 0;
}

bool Animation::ChangeFrame() {
	time += TimeSystem::instance()->GetTimeBetweenFrame();
	std::cout << time << std::endl;
	if ((time / 1000.0f) >= period) {
		time = 0.0;
		return true;
	}
	return false;
}

void Animation::SetFramePeriod(double time) {
	period = time;
}

std::string Animation::GetTexture() {
	return texturePath;
}

GLfloat* Animation::GetCurrentFrame() {
	currentFrame %= frame;
	float tmp = currentFrame * frameWidth;
	
	GLfloat* newTexData = new GLfloat[8];
	newTexData[0] = 0.0f + tmp;
	newTexData[1] = 0.0f;
	newTexData[2] = frameWidth + tmp;
	newTexData[3] = 0.0f;
	newTexData[4] = frameWidth + tmp;
	newTexData[5] = 1.0f;
	newTexData[6] = 0.0f + tmp;
	newTexData[7] = 1.0f;
	return newTexData;
}

void Animation::NextFrame() {
	currentFrame++;
}

bool Animation::Finished() {
	if (currentFrame == frame) 
		return true;
	return false;
}

void Animation::ResetAnimation() {
	currentFrame = 0;
	time = 0.0;
}