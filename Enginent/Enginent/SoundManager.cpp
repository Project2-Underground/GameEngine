#include "SoundManager.h"
#include "Game.h"
#include <GL\glew.h>

SoundManager* SoundManager::_instance = 0;

SoundManager::SoundManager() {
	soundEngine = createIrrKlangDevice();
}

void SoundManager::Init()
{
	incVol = 0.1f;
	masterVol = 1.0f;
	bgmVol = 1.0f;
	sfxVol = 1.0f;
	// BGM
	bgms["MainScreen"] = createSound("Sound/BG_Mainscreen.mp3");

	// SFX
	sfxs["Walking"] = createSound("Sound/walking_sound.mp3");
	sfxs["OpenDoor"] = createSound("Sound/open_door_3.mp3");
	sfxs["Locked"] = createSound("Sound/door_lockEdit.mp3");
	sfxs["CollectNote"] = createSound("Sound/Collect_Note.mp3");
	sfxs["Pickup"] = createSound("Sound/pick_up_1.mp3");
	sfxs["OpenDrawer"] = createSound("Sound/open_drawer.mp3");
	sfxs["OpenCabinet"] = createSound("Sound/open_closet.mp3");
}

ISoundSource* SoundManager::createSound(const char* filename)
{
	ISoundSource* iss = soundEngine->addSoundSourceFromFile(filename);
	return iss;
}

void SoundManager::playSound(SoundType t, std::string sound, bool loop)
{
	switch (t)
	{
	case BGM:
		soundEngine->play2D(bgms[sound], loop);
		break;
	case SFX:
		soundEngine->play2D(sfxs[sound], loop);
		break;
	default:
		break;
	}
}

void SoundManager::pause(bool b) {
	soundEngine->setAllSoundsPaused(b);
}

void SoundManager::pause(SoundType t, std::string sound) {
	switch (t)
	{
	case BGM:
		soundEngine->setAllSoundsPaused(bgms[sound]);
		break;
	case SFX:
		soundEngine->setAllSoundsPaused(sfxs[sound]);
		break;
	default:
		break;
	}
}

void SoundManager::stop(SoundType t,std::string sound) {
	switch (t)
	{
	case BGM:
		soundEngine->stopAllSoundsOfSoundSource(bgms[sound]);
		break;
	case SFX:
		soundEngine->stopAllSoundsOfSoundSource(sfxs[sound]);
		break;
	default:
		break;
	}
}

void SoundManager::stopAllSounds() {
	soundEngine->stopAllSounds();
}

void SoundManager::upVolume(SoundType t) {
	switch (t)
	{
	case BGM:
		if (bgmVol < 1.0f) {
			bgmVol += incVol;
			for (auto sound : bgms)
				if (sound.second != nullptr)
					sound.second->setDefaultVolume(bgmVol);
		}
		break;
	case SFX:
		if (sfxs.begin()->second->getDefaultVolume() < 1.0f) {
			sfxVol += incVol;
			for (auto sound : sfxs)
				if (sound.second != nullptr)
					sound.second->setDefaultVolume(sfxVol);
		}
		break;
	default:
		if (soundEngine->getSoundVolume() <= 1.0f) {
			masterVol += incVol;
			soundEngine->setSoundVolume(masterVol);
		}
		break;
	}
}

void SoundManager::downVolume(SoundType t) {
	switch (t)
	{
	case BGM:
		if (bgms.begin()->second->getDefaultVolume() >= 0.0f)
			for (auto sound : bgms)
				if (sound.second != nullptr)
					sound.second->setDefaultVolume(sound.second->getDefaultVolume() - incVol);
		break;
	case SFX:
		if (sfxs.begin()->second->getDefaultVolume() >= 0.0f) 
			for (auto sound : sfxs)
				if(sound.second != nullptr)
					sound.second->setDefaultVolume(sound.second->getDefaultVolume() - incVol);
		break;
	default:
		if (soundEngine->getSoundVolume() >= 0.0f) 
			soundEngine->setSoundVolume(soundEngine->getSoundVolume() - incVol);
		break;
	}
}

void SoundManager::toggleMute(SoundType t) {
	float vol;
	switch (t)
	{
	case BGM:
		bgmMute = !bgmMute;
		vol = bgmVol;
		if (bgmMute)
			vol = 0.0f;
		for (auto sound : bgms)
			if (sound.second != nullptr)
				sound.second->setDefaultVolume(vol);

		break;
	case SFX:
		sfxMute = !sfxMute;
		vol = sfxVol;
		if (sfxMute)
			vol = 0.0f;
		if (sfxs.begin()->second->getDefaultVolume() > 0.0f)
			vol = 0.0f;
		for (auto sound : sfxs)
			if (sound.second != nullptr)
				sound.second->setDefaultVolume(vol);

		break;
	default:
		masterMute = !masterMute;
		vol = masterVol;
		if (masterMute)
			vol = 0.0f;
		soundEngine->setSoundVolume(vol);
		break;
	}
}

float SoundManager::getVolume(SoundType t) {
	switch (t)
	{
	case BGM:
		return bgmVol;
	case SFX:
		return sfxVol;
	case MASTER:
		return masterVol;
	}
	return 0.0f;
}

float SoundManager::getMute(SoundType t) {
	switch (t)
	{
	case BGM:
		return bgmMute;
	case SFX:
		return sfxMute;
	case MASTER:
		return masterMute;
	}
	return false;
}
void SoundManager::setVolume(SoundType t, float vol) {
	switch (t)
	{
	case BGM:
		for (auto sound : bgms)
			if (sound.second != nullptr)
				sound.second->setDefaultVolume(vol);
	case SFX:
		for (auto sound : sfxs)
			if(sound.second != nullptr)
				sound.second->setDefaultVolume(vol);
	case MASTER:
		soundEngine->setSoundVolume(vol);
	}
}

void SoundManager::setMute(SoundType t, bool m) {
	switch (t)
	{
	case BGM:
		bgmMute = m;
		if (m) 
			for (auto sound : bgms)
				if (sound.second != nullptr)
					sound.second->setDefaultVolume(0);
		
	case SFX:
		sfxMute = m;
		if (m)
			for (auto sound : sfxs)
				if (sound.second != nullptr)
					sound.second->setDefaultVolume(0);
	case MASTER:
		masterMute = m;
		if (m)
			soundEngine->setSoundVolume(0);
	}
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
		_instance->Init();
	}
	return _instance;
}