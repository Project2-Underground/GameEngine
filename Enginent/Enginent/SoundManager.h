#pragma once

#include "irrKlang.h"
#include <map>

using namespace irrklang;

class SoundManager {
private:
	ISoundEngine *soundEngine;
	std::map<std::string, ISound*> sound;
	static ISoundEngine* _instance;
protected:
	SoundManager();
public:
	static ISoundEngine* GetInstance();
	void playBg(const char* filename);
	void playSFX(const char* filename);
	void pause();
	void unPause();
	void stopBg();
	void upVolume();
	void downVolume();
	void toggleMute();
	~SoundManager();
};