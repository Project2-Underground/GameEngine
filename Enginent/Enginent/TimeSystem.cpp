#include "TimeSystem.h"


void FrameInit() {
	prevTime = SDL_GetTicks();
}

void FrameStart() {
	currTime = SDL_GetTicks();
}

double GetTimeBetweenFrame() {
	return currTime - prevTime;
}

void FrameEnd() {
	prevTime = currTime;
}