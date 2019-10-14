#pragma once

#include "irrKlang.h"

using namespace irrklang;

class SoundManager {
	ISoundEngine *soundEngine;
public:
	SoundManager();
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