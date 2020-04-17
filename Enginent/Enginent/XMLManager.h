#pragma once

#include <pugixml.hpp>
#include <string>
#include "Level.h"
#include "ImageObject.h"
#include "InfoPhone.h"
#include "Item.h"

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
	void GenerateImage(pugi::xml_node, Room*, std::string);
	void GenerateInteractObj(pugi::xml_node, Room*);
	void GenerateDoor(pugi::xml_node, Room*);
	void GenerateNPC(pugi::xml_node, Room*);
	void CreateObject(ImageObject* tmp, pugi::xml_node);

	void LoadObjSpecialActions(std::string, Level*);
	void LoadItems(std::vector<Item*>&);

	int GetLevelNumber(std::string); 
	std::string GetFirstRoomName();
	void SaveGame(std::string);
	void LoadFromSave(std::string);

	// chat doc
	void LoadChats(std::string, std::map<std::string, ChatInfo>&);
	std::string GetMessage(std::string, int);
	// note doc
	void LoadNotes(std::string, std::map<std::string, UIObject*>&);

	// script doc
	//std::map<std::string, Script> GetScript(std::string);

	// save game option
	void SaveGameOptions();
	void LoadGameOptions();

	~XMLManager();
};