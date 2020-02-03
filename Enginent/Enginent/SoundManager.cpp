#include "SoundManager.h"
#include "Game.h"
#include <GL\glew.h>

SoundManager* SoundManager::_instance = 0;

SoundManager::SoundManager() {
	soundEngine = createIrrKlangDevice();
}

void SoundManager::Init()
{
	sounds["MainScreen"] = createSound("Sound/BG_Mainscreen.mp3");
	sounds["Walking"] = createSound("Sound/walking_sound.mp3");
	sounds["Door"] = createSound("Sound/Door_sound.mp3");
	sounds["Locked"] = createSound("Sound/Locked_sound.mp3");
}

ISoundSource* SoundManager::createSound(const char* filename)
{
	return soundEngine->addSoundSourceFromFile(filename);
}

void SoundManager::playSound(std::string sound, bool loop) 
{
	soundEngine->play2D(sounds[sound], loop);
}

void SoundManager::playSFX(std::string sound)
{
	soundEngine->play2D(sounds[sound], false);
}

void SoundManager::pause(std::string sound) {
	soundEngine->setAllSoundsPaused(sounds[sound]);
}

void SoundManager::stop(std::string sound) {
	soundEngine->stopAllSoundsOfSoundSource(sounds[sound]);
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