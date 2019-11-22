#pragma once

#include "irrKlang.h"
#include <map>

using namespace irrklang;

class SoundManager {
private:
	ISoundEngine *soundEngine;
	std::map<std::string, ISound*> sounds;
	static SoundManager* _instance;
protected:
	SoundManager();
public:
	static SoundManager* GetInstance();
	ISound* createSound(const char* filename, bool loop, bool pauseAtStart);
	void Init();
	void playSound(std::string);
	void pause(std::string);
	void stop(std::string);
	void stopAllSounds();
	void upVolume();
	void downVolume();
	void toggleMute();
	~SoundManager();
};