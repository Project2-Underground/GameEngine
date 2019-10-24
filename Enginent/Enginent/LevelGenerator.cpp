#include "LevelGenerator.h"
#include "Game.h"

void RoomGenerator::GenerateRoom(std::string filename) {
	LoadFile(filename);
	pugi::xml_node node = doc.first_child().first_child();	// get the first node of the root
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		// loop through headers
		for (pugi::xml_node childChild = child.first_child(); childChild; childChild = child.next_sibling()) {
			// get attributes
			// createObject
			int type = childChild.find_attribute("type").as_int();
			std::string texture = childChild.find_attribute("texture").as_string();
			int sizeX = childChild.find_attribute("sizeX").as_int();
			int sizeY = childChild.find_attribute("sizeY").as_int();
			glm::vec3 pos = glm::vec3(childChild.find_attribute("posX").as_float(),
									  childChild.find_attribute("posY").as_float(),
									  1.0f);
			std::string dialoguePath = childChild.find_attribute("dialogue").as_string();

			Game::GetInstance()->createObject(type, texture, sizeX, sizeY, pos, dialoguePath);
		}
	}
}