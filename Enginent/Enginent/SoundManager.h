#pragma once

#include "irrKlang.h"
#include <map>

using namespace irrklang;

enum SoundType {
	BGM = 0,
	SFX,
	MASTER
};

class SoundManager {
private:
	ISoundEngine *soundEngine;
	std::map<std::string, ISoundSource*> sfxs;
	std::map<std::string, ISoundSource*> bgms;
	static SoundManager* _instance;
	ik_f32 incVol;

	float masterVol;
	float bgmVol;
	float sfxVol;

	bool bgmMute;
	bool sfxMute;
	bool masterMute;
protected:
	SoundManager();
public:
	static SoundManager* GetInstance();
	ISoundSource* createSound(const char* filename);
	void Init();
	void playSound(SoundType, std::string, bool loop = false);
	void pause(SoundType, std::string);
	void pause(bool b);
	void stop(SoundType, std::string);
	void stopAllSounds();
	void upVolume(SoundType);
	void downVolume(SoundType);
	void toggleMute(SoundType);

	float getVolume(SoundType);
	void setVolume(SoundType, float);
	float getMute(SoundType);
	void setMute(SoundType, bool);
	~SoundManager();
};