#include "LevelGenerator.h"


void RoomGenerator::GenerateRoom(std::string filename) {
	LoadFile(filename);
	pugi::xml_node node = doc.first_child().first_child();	// get the first node of the root
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		// loop through headers
		for (pugi::xml_node childChild = child.first_child(); childChild; childChild = child.next_sibling()) {
			// get attributes
			// createObject
		}
	}
}