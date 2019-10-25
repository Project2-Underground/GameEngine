#include "LevelGenerator.h"
#include "Game.h"
#include <iostream>

void RoomGenerator::GenerateRoom(std::string filename) {
	/* generate room base on the xml doc */
	LoadFile(filename);
	pugi::xml_node node = doc.first_child().first_child();	// get the first node of the root
	for (pugi::xml_node child = node.first_child(); child != node.last_child(); child = child.next_sibling()) {
		std::cout << "in";
		// loop through headers
		int type = child.attribute("type").as_int();
		for (pugi::xml_node childChild = child.first_child(); childChild; childChild = child.next_sibling()) {
			// get attributes
			// createObject
			std::string texture = childChild.attribute("texture").as_string();
			int sizeX = childChild.attribute("sizeX").as_int();
			int sizeY = childChild.attribute("sizeY").as_int();
			glm::vec3 pos = glm::vec3(childChild.attribute("posX").as_float(),
									  childChild.attribute("posY").as_float(),
									  1.0f);
			std::string dialoguePath = childChild.attribute("dialogue").as_string();

			Game::GetInstance()->createObject(type, texture, sizeX, sizeY, pos, dialoguePath);
		}
	}
}