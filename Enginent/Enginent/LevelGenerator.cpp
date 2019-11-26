#include "LevelGenerator.h"
#include "Game.h"
#include <iostream>

void RoomGenerator::GenerateRoom(std::string filename) {
	/* generate room base on the xml doc */
	if (LoadFile(filename)) {
		pugi::xml_node object = doc.child("room");	// get the first node of the root
		for (pugi::xml_node_iterator child = object.begin(); child != object.end(); child++) {
			//std::cout << "in";
			// loop through headers
			int type = child->attribute("type").as_int();
			for (pugi::xml_node_iterator aChild = child->begin(); aChild != child->end(); aChild++) {
				// get attributes
				// createObject
				std::string texture = aChild->attribute("texture").as_string();
				int sizeX = aChild->attribute("sizeX").as_int();
				int sizeY = aChild->attribute("sizeY").as_int();
				glm::vec3 pos = glm::vec3(aChild->attribute("posX").as_float(),
					aChild->attribute("posY").as_float(),
					1.0f);
				std::vector<std::string> dialogueVec;
				if (aChild->child("dialogue")) {
					for (pugi::xml_node_iterator dialogue = aChild->begin(); dialogue != aChild->end(); dialogue++) {
						dialogueVec.push_back(dialogue->text().get());
					}
				}
				//Game::GetInstance()->createObject(type, texture, sizeX, -sizeY, pos, dialogueVec);
			}
		}
	}
}