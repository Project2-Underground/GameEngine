#pragma once

#include <pugixml.hpp>
#include <string>
#include "ImageObject.h"
#include "Door.h"
#include "Level.h"

class XMLManager {
	static XMLManager* _instance;
	pugi::xml_document doc;
	pugi::xml_document chatDoc;

	XMLManager();
	bool LoadFile(std::string filename);
public:
	static XMLManager* GetInstance();

	// doc
	void GenerateRoom(std::string, std::map<std::string, Room*>& rooms);
	void GenerateImage(pugi::xml_node, std::vector<DrawableObject*>&, std::string);
	void GenerateInteractObj(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateDoor(pugi::xml_node, std::vector<DrawableObject*>&, std::map<std::string, Door*>&);
	void GenerateItem(pugi::xml_node, std::vector<DrawableObject*>&);
	void GenerateNPC(pugi::xml_node, std::vector<DrawableObject*>&);
	void GeneratePuzzle(pugi::xml_node, std::vector<DrawableObject*>&);
	void CreateObject(ImageObject* tmp, pugi::xml_node);

	int GetLevelNumber(std::string);
	void SaveGame(std::string);
	void LoadFromSave(std::string);

	// chat doc
	void GetChat(std::string, ImageObject*);
	std::string GetMessage(std::string, int);

	~XMLManager();
};