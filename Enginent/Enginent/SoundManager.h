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

	bool loadingFromSave;
protected:
	SoundManager();
public:
	static SoundManager* GetInstance();
	ISoundSource* createSound(const char* filename);
	void Init();
	void playSound(SoundType, std::string, bool loop = false);
	void PauseBGM(bool b);
	void PauseSFX(bool b);
	void pause(bool b);
	void stop(SoundType, std::string);
	void stopAllSounds();
	void toggleMute(SoundType);
	void StopBGM(std::string name);
	void StopCurrentBGM();

	float getMute(SoundType);
	void setMute(SoundType, bool);
	void LoadMute();
	~SoundManager();

	std::string currentBGM;
};