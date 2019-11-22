#include "LevelGenerator.h"
#include "Game.h"
#include <iostream>

void RoomGenerator::GenerateRoom(std::string filename) {
	/* generate room base on the xml doc */
	if (LoadFile(filename)) {
		pugi::xml_node items = doc.child("room");	// get the first node of the root
		for (pugi::xml_node_iterator child = items.begin(); child != items.end(); child++) {
			// loop through headers
			int type = child->attribute("type").as_int();
			// have not implemented type 1
			if (type <= 1) {
				for (pugi::xml_node_iterator aChild = child->begin(); aChild != child->end(); aChild++) {
					// get attributes
					// createObject
					std::string texture = aChild->attribute("texture").as_string();
					int sizeX = aChild->attribute("sizeX").as_int();
					int sizeY = aChild->attribute("sizeY").as_int();
					glm::vec3 pos = glm::vec3(aChild->attribute("posX").as_float(),
						aChild->attribute("posY").as_float(),
						1.0f);
					std::vector<std::string> dialogues;
					dialogues.push_back(aChild->attribute("dialogue").as_string());

					Game::GetInstance()->createObject(type, texture, sizeX, -sizeY, pos, (IneractTypeList)type, dialogues);
				}
			}
		}
	}
}