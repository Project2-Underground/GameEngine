#pragma once

#include "irrKlang.h"
#include <map>

using namespace irrklang;

class SoundManager {
private:
	ISoundEngine *soundEngine;
	std::map<std::string, ISoundSource*> sounds;
	static SoundManager* _instance;
protected:
	SoundManager();
public:
	static SoundManager* GetInstance();
	ISoundSource* createSound(const char* filename);
	void Init();
	void playSound(std::string, bool loop);
	void playSFX(std::string);
	void pause(std::string);
	void stop(std::string);
	void stopAllSounds();
	void upVolume();
	void downVolume();
	void toggleMute();
	~SoundManager();
};