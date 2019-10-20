#include <SDL.h>

double prevTime = 0.0f;
double currTime = 0.0f;

void FrameInit();
void FrameStart();
double GetTimeBetweenFrame();
void FrameEnd();