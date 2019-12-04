#include "xmlFileManager.h"
#include <iostream>

bool xmlManager::LoadFile(std::string filename) {
	pugi::xml_parse_result result = doc.load_file(filename.c_str(), pugi::parse_default|pugi::parse_declaration);
	if (!result) {
		std::cout << "cannot open file\n";
		return false;
	}
	return true;
}

void xmlManager::SaveFile(std::string filename) {
	doc.save_file(filename.c_str());
}
