#include "Animation.h"
#include "TimeSystem.h"

Animation::Animation(ImageObject* ref, std::string name, std::string TexturePath) {
	this->ref = ref;
	this->animationName = name;
	this->texturePath = TexturePath;
}

void Animation::SetFrame(int frame) {
	this->frame = frame;
	frameWidth = 1.0f / (float)frame;
	currentFrame = 0;
}

bool Animation::ChangeFrame() {
	time += GetTimeBetweenFrame();
	if ((time / 1000) >= period)
		return true;
	return false;
}

void Animation::SetFramePeriod(double time) {
	period = time;
}

void Animation::SetTexture() {
	ref->SetTexture(texturePath);
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