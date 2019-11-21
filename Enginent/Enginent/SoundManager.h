#pragma once

#include "irrKlang.h"
#include <map>

using namespace irrklang;

class SoundManager {
private:
	ISoundEngine *soundEngine;
	std::map<std::string, ISound*> sound;
	static SoundManager* _instance;
protected:
	SoundManager();
public:
	static SoundManager* GetInstance();
	ISound* createSound(const char* filename, bool loop, bool pauseAtStart);
	void playSound(ISound*);
	void pause(ISound*);
	void stop(ISound*);
	void stopAllSounds();
	void upVolume();
	void downVolume();
	void toggleMute();
	~SoundManager();
};