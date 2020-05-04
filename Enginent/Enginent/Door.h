#pragma once

#include "InteractObj.h"

class Door : public InteractableObj {
protected:
	std::string nextRoom;
	std::string nextDoor;
	unsigned int openTexture;
public:
	Door(std::string nextroom, std::string nextdoor);
	void SetOpenTexture(std::string texture);
	void SetOpenTexture(unsigned int texture) { openTexture = texture; }
	void Open();
	void action();
};

class WallDoor : public Door {
public:
	WallDoor(std::string room, std::string door) :Door(room, door) { used = false; }
	void action();
};

class EliasDoor :public Door {
	std::string dialogueAfterOpen;
public:
	EliasDoor(std::string room, std::string door) :Door(room, door) { }
	void action();
	//void Update(); 
};

class ChangeLevelDoor : public Door {
	int nextLevel;
public:
	ChangeLevelDoor(int nextLvl) :Door("", "") { nextLevel = nextLvl; }
	void action();
};