#include "LevelGenerator.h"
#include "Game.h"
#include <iostream>

void RoomGenerator::GenerateRoom(std::string filename) {
	/* generate room base on the xml doc */
	if (LoadFile(filename)) {
		pugi::xml_node object = doc.child("room");	// get the first node of the root
		for (pugi::xml_node_iterator child = object.begin(); child != object.end(); child++) {
			std::cout << "in";
			 //loop through headers
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
					for (pugi::xml_node_iterator dialogue = aChild->child("dialogue").begin(); dialogue != aChild->child("dialogue").end(); dialogue++) {
						dialogueVec.push_back(dialogue->child_value());
					}
				}
				Game::GetInstance()->createObject((objectType)type, texture, sizeX, -sizeY, pos, dialogueVec);
			}
		}
	}
}

void RoomGenerator::GenerateRoom(std::string filename, std::string roomNo, std::vector<DrawableObject*> objects) {
	if (LoadFile(filename)) {
		pugi::xml_node room = doc.child("rooms").child(roomNo.c_str());

		for (int i = 0; i < objects.size(); i++) {
			pugi::xml_node object = room.child(objects[i]->object_name.c_str());

			// get attributes
			std::string texture = object.attribute("texture").as_string();
			int sizeX = object.attribute("sizeX").as_int();
			int sizeY = object.attribute("sizeY").as_int();
			int posX = object.attribute("posX").as_float();
			int posY = object.attribute("posY").as_float();
			std::vector<std::string> dialogues;

			if (pugi::xml_node dialogue = object.child("dialogue")) {
				for (pugi::xml_node_iterator d = dialogue.begin(); d != dialogue.end(); d++) {
					dialogues.push_back(d->child_value());
				}
			}

			Game::GetInstance()->SetObjectAttributes((ImageObject*)objects[i], texture, sizeX, sizeY, glm::vec3(posX, posY, 1));
		}
	}
}