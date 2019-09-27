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

void SoundManager::stopBg() {
	soundEngine->stopAllSounds();
}