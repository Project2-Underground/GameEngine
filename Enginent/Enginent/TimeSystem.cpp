#include "TimeSystem.h"
#include <iostream>

TimeSystem* TimeSystem::_instance = nullptr;

TimeSystem* TimeSystem::instance() {
	if (_instance == nullptr)
		_instance = new TimeSystem();
	return _instance;
}

void TimeSystem::FrameInit() {
	prevTime = SDL_GetTicks();
}

void TimeSystem::FrameStart() {
	currTime = SDL_GetTicks();
}

double TimeSystem::GetTimeBetweenFrame() {
	return currTime - prevTime;
}

float TimeSystem::GetDT() {
	return (currTime - prevTime) / 1000.0f;

}
void TimeSystem::FrameEnd() {
	prevTime = currTime;
}