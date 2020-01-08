#pragma once

#include <pugixml.hpp>
#include <string>
#include "ImageObject.h"
#include "Door.h"
#include "Level.h"

class LevelGenerator {
	static LevelGenerator* _instance;
	pugi::xml_document doc;
	bool LoadFile(std::string filename);
public:
	static LevelGenerator* GetInstance();

	void GenerateRoom(std::string, std::map<std::string, Room*>& rooms);
	void GenerateBackground(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateInteractObj(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateDoor(pugi::xml_node, std::vector<DrawableObject*>&, std::map<std::string, Door*>&);
	void GenerateItem(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateNPC(pugi::xml_node, std::vector<DrawableObject*>&);
	void GeneratePuzzle(pugi::xml_node, std::vector<DrawableObject*>&);

	int GetLevelNumber(std::string);
	void SaveGame(std::string);
	void LoadFromSave(std::string);

	void CreateObject(ImageObject* tmp, pugi::xml_node);

	~LevelGenerator();
};