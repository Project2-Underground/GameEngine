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
	return node.child(nodeName.c_str());
}
char* xmlManager::GetPcData(pugi::xml_node node, int childPos) {
	int i = 0;
	pugi::xml_node nextNode = node.first_child();
	while (i < childPos && nextNode) {
		i++;
		nextNode = nextNode.next_sibling();
	}
	return (char*)nextNode.text().get();
}
pugi::xml_attribute xmlManager::GetAttribute(pugi::xml_node node, std::string attr) {
	return node.attribute(attr.c_str());
}

void xmlManager::ChangeAttributeValue(pugi::xml_attribute attr, std::string value) {
	attr.set_value(value.c_str());
}
void xmlManager::ChangeTextValue(pugi::xml_node node, int childPos, std::string newValue) {
	int i = 0;
	pugi::xml_node nextNode = node.first_child();
	while (i < childPos && nextNode) {
		i++;
		nextNode = nextNode.next_sibling();
	}
	nextNode.text().set(newValue.c_str());
}