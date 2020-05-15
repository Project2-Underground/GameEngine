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
	bgms["Mainfloor2"] = createSound("Sound/mainfloor2.mp3");
	bgms["Mainfloor3"] = createSound("Sound/mainfloor3.mp3");
	bgms["HallRoom"] = createSound("Sound/Hall_Room_BG_Music.mp3");
	bgms["EndTheme"] = createSound("Sound/Ending_Theme.mp3");
	bgms["Buildng3"] = createSound("Sound/Building3_BGM.mp3");
	bgms["talkingAmbient"] = createSound("Sound/people-talking-in-background-ambience.mp3");

	// normal SFX
	sfxs["Walking"] = createSound("Sound/walking_sound.mp3");
	sfxs["OpenDoor"] = createSound("Sound/open_door_3.mp3");
	sfxs["Locked"] = createSound("Sound/door_lockEdit.mp3");
	sfxs["CollectNote"] = createSound("Sound/Collect_Note.mp3");
	sfxs["Pickup"] = createSound("Sound/pick_up_1.mp3");
	sfxs["OpenDrawer"] = createSound("Sound/open_drawer.mp3");
	sfxs["OpenCabinet"] = createSound("Sound/open_closet.mp3");
	sfxs["Key"] = createSound("Sound/Key.mp3");
	sfxs["Click_Button"] = createSound("Sound/Click_Button.mp3");
	sfxs["Click_ButtonUI"] = createSound("Sound/Click_ButtonUI.mp3");
	sfxs["BasementNumpad"] = createSound("Sound/BasementNumpadSound.mp3");
	sfxs["Click"] = createSound("Sound/Click.mp3");
	sfxs["Puzzle25DoorOpen"] = createSound("Sound/Puzzle_2-5_DoorOpen.mp3");
	sfxs["Puzzle_3_finish_sound"] = createSound("Sound/Puzzle_2-Puzzle_3_finish_sound.mp3");

	// cutscene SFX
	sfxs["Reload"] = createSound("Sound/Reload.mp3");
	sfxs["Butler_Shoot"] = createSound("Sound/Butler_Shoot.mp3");
	sfxs["fire"] = createSound("Sound/fire_with_scream.mp3");
	sfxs["water"] = createSound("Sound/Water.mp3");
	sfxs["chainsaw"] = createSound("Sound/chainsaw.mp3");
}

ISoundSource* SoundManager::createSound(const char* filename)
{
	ISoundSource* iss = soundEngine->addSoundSourceFromFile(filename);
	return iss;
}

void SoundManager::playSound(SoundType t, std::string sound, bool loop)
{
	if (!sound.empty()) {
		if (!loadingFromSave) {
			switch (t)
			{
			case BGM:
				soundEngine->play2D(bgms[sound], true); 
				if(sound != "talkingAmbient")
					currentBGM = sound;
				break;
			case SFX:
				soundEngine->play2D(sfxs[sound], loop);
				break;
			default:
				break;
			}
		}
	}
}

void SoundManager::pause(bool b) {
	soundEngine->setAllSoundsPaused(b);
}
void SoundManager::PauseBGM(bool b) {
	if (b) {
		for (auto sound : bgms)
			if (sound.second != nullptr) {
				soundEngine->stopAllSoundsOfSoundSource(sound.second);
				sound.second->setDefaultVolume(0);
			}
	}
	else {
		for (auto sound : bgms)
			if (sound.second != nullptr) {
				sound.second->setDefaultVolume(1);
			}
		soundEngine->play2D(bgms[currentBGM], true);
		if (currentBGM == "Mainfloor2" || currentBGM == "Mainfloor3") {
			soundEngine->play2D(bgms["talkingAmbient"], true);
		}
	}
}

void SoundManager::PauseSFX(bool b) {
	if (b) {
		for (auto sound : sfxs)
			if (sound.second != nullptr) {
				soundEngine->stopAllSoundsOfSoundSource(sound.second);
				sound.second->setDefaultVolume(0);
			}
	}
	else {
		for (auto sound : sfxs)
			if (sound.second != nullptr) {
				sound.second->setDefaultVolume(1);
			}
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

void SoundManager::StopCurrentBGM() {
	soundEngine->stopAllSoundsOfSoundSource(bgms[currentBGM]);
	soundEngine->stopAllSoundsOfSoundSource(bgms["talkingAmbient"]);
	currentBGM.clear();
}
void SoundManager::StopBGM(std::string name) {
	soundEngine->stopAllSoundsOfSoundSource(bgms[name]);
}

void SoundManager::stopAllSounds() {
	soundEngine->stopAllSounds();
	currentBGM.clear();
}

void SoundManager::toggleMute(SoundType t) {
	float vol;
	switch (t)
	{
	case BGM:
		bgmMute = !bgmMute;
		PauseBGM(bgmMute);
		break;
	case SFX:
		sfxMute = !sfxMute;
		PauseSFX(sfxMute);
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

void SoundManager::LoadMute() {
	loadingFromSave = !loadingFromSave;
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