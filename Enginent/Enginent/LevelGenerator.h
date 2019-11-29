#pragma once

#include "xmlFileManager.h"
#include "DrawableObject.h"

class RoomGenerator : public xmlManager {
public:
	void GenerateRoom(std::string, std::string roomNo, std::vector<DrawableObject*>);
	void GenerateRoom(std::string);
};