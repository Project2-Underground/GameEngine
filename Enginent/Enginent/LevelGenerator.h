#pragma once

#include "xmlFileManager.h"
#include "ImageObject.h"

class RoomGenerator : public xmlManager {
public:
	void GenerateRoom(std::string, std::string roomNo, std::vector<DrawableObject*> &objects);
	//void GenerateRoom(std::string);
	void GenerateBackground(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateInteractObj(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateDoor(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateItem(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateNPC(pugi::xml_node, std::vector<DrawableObject*>&);

	void CreateObject(ImageObject* tmp, pugi::xml_node);
};