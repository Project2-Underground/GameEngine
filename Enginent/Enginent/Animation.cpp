#include "Animation.h"
#include "TimeSystem.h"
#include "Game.h"
#include <iostream>

Animation::Animation(std::string name, std::string TexturePath, bool leaveTime) {
	this->animationName = name;
	this->texture = Game::GetInstance()->GetRenderer()->LoadTexture(TexturePath);;
	this->hasLeaveTime = leaveTime;
}

void Animation::SetFrame(int frame) {
	this->frame = frame;
	frameWidth = 1.0f / (float)frame;
	currentFrame = 0;
}

bool Animation::ChangeFrame() {
	time += TimeSystem::instance()->GetTimeBetweenFrame();
	if ((time / 1000.0f) >= period) {
		time = 0.0;
		return true;
	}
	return false;
}

void Animation::SetFramePeriod(double time) {
	period = time;
}

unsigned int Animation::GetTexture() {
	return texture;
}

void Animation::GetCurrentFrame(GLfloat* texData) {
	currentFrame %= frame;
	float tmp = currentFrame * frameWidth;
	
	texData[0] = 0.0f + tmp;
	texData[1] = 0.0f;
	texData[2] = frameWidth + tmp;
	texData[3] = 0.0f;
	texData[4] = frameWidth + tmp;
	texData[5] = 1.0f;
	texData[6] = 0.0f + tmp;
	texData[7] = 1.0f;

	changed = true;
}

void Animation::NextFrame() {
	currentFrame++;
	changed = false;
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