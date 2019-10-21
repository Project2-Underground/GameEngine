#include "xmlFileManager.h"
#include <iostream>

bool xmlManager::LoadFile(std::string filename) {
	pugi::xml_parse_result result = doc.load_file(filename.c_str(), pugi::parse_default|pugi::parse_declaration);
	if (!result)
		return false;
		//std::cout << "Error to load file" << std::endl;
	return true;
}

void xmlManager::SaveFile(std::string filename) {
	doc.save_file(filename.c_str());
}

pugi::xml_node xmlManager::GetNode(pugi::xml_node node, std::string nodeName) {
	return node.find_node(nodeName);
}
pugi::xml_node xmlManager::GetPcData(pugi::xml_node node, int ptr) {
	/*for (int i = 0; i < ptr, i++) {
		pcData = 
	}*/
}
pugi::xml_attribute xmlManager::GetAttribute(pugi::xml_node node, std::string attr) {

}

void xmlManager::ChangeAttributeValue(pugi::xml_attribute attr, std::string value) {

}
void xmlManager::SetTextValue(pugi::xml_node node, int ptr, std::string newValue) {

}