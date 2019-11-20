#pragma once
#include "Game.h"

class Scene {
	private:
		int boundX;
		int boundY;
	public:
		Scene();
		~Scene();
		void setBound(int x, int y);
		void loadScene(std::string sceneName);
};