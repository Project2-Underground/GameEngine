#include "SoundManager.h"
#include <GL\glew.h>


SoundManager::SoundManager() {
	soundEngine = createIrrKlangDevice();
}

void SoundManager::playBg(const char *filename) {
	soundEngine->play2D(filename, GL_TRUE);
}

void SoundManager::playSFX(const char* filename) {
	soundEngine->play2D(filename, GL_FALSE);
}

void SoundManager::pause() {
	soundEngine->setAllSoundsPaused();
}

void SoundManager::unPause() {
	soundEngine->setAllSoundsPaused(false);
}

void SoundManager::stopBg() {
	soundEngine->stopAllSounds();
}

void SoundManager::upVolume() {
	ik_f32 vol = 0.1f;
	if (soundEngine->getSoundVolume() <= 1.0f) {
		soundEngine->setSoundVolume(soundEngine->getSoundVolume() + vol);
	}
}

void SoundManager::downVolume() {
	ik_f32 vol = 0.1f;
	if (soundEngine->getSoundVolume() >= 0.0f) {
		soundEngine->setSoundVolume(soundEngine->getSoundVolume() - vol);
	}
}

void SoundManager::toggleMute() {
	ik_f32 vol = 1.0f;
	if (soundEngine->getSoundVolume() == 1.0f)
		ik_f32 vol = 0.0f;

	soundEngine->setSoundVolume(vol);
}

SoundManager::~SoundManager() {
	soundEngine->stopAllSounds();
	delete soundEngine;
}