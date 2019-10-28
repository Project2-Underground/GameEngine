#include <SDL.h>

class TimeSystem {
	static TimeSystem* _instance;
public:
	static TimeSystem* instance();
	double prevTime;
	double currTime;
	void FrameInit();
	void FrameStart();
	double GetTimeBetweenFrame();
	void FrameEnd();
};