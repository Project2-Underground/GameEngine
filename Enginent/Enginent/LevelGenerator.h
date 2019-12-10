#pragma once

#include "xmlFileManager.h"
#include "ImageObject.h"
#include "Door.h"
#include "Level.h"

class LevelGenerator : public xmlManager {
public:
	//void GenerateRoom(std::string, std::string roomNo, std::vector<DrawableObject*> &objects);
	void GenerateRoom(std::string, std::map<std::string, Room*> &rooms);
	//void GenerateRoom(std::string);
	void GenerateBackground(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateInteractObj(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateDoor(pugi::xml_node, std::vector<DrawableObject*>&, std::map<std::string, Door*>&);
	void GenerateItem(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateNPC(pugi::xml_node, std::vector<DrawableObject*>&);
	void GeneratePuzzle(pugi::xml_node, std::vector<DrawableObject*>&);

	void CreateObject(ImageObject* tmp, pugi::xml_node);
};