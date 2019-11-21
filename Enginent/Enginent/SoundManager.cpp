#include "SoundManager.h"
#include <GL\glew.h>

SoundManager* SoundManager::_instance = 0;

SoundManager::SoundManager() {
	soundEngine = createIrrKlangDevice();
}

ISound* SoundManager::createSound(const char* filename, bool loop, bool pauseAtStart)
{
	return soundEngine->play2D(filename, loop, pauseAtStart);
}

void SoundManager::playSound(ISound* sound) {
	sound->setIsPaused(false);
}

void SoundManager::pause(ISound* sound) {
	sound->setIsPaused(true);
}

void SoundManager::stop(ISound* sound) {
	sound->setIsPaused(true);
	sound->setPlayPosition(0);
}

void SoundManager::stopAllSounds() {
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
	soundEngine->drop();
	delete soundEngine;
	delete _instance;
}

SoundManager* SoundManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SoundManager();
	}
	return _instance;
}