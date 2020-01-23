#include "LevelGenerator.h"
#include "Item.h"
#include "Game.h"
#include "Level.h"

#include <iostream>

XMLManager* XMLManager::_instance = nullptr;

XMLManager* XMLManager::GetInstance() {
	if (_instance == nullptr)
		_instance = new XMLManager();
	return _instance;
}

XMLManager::XMLManager() {
	//load all constant files
	chatDoc.load_file("save/chats.xml", pugi::parse_default | pugi::parse_declaration);
}

bool XMLManager::LoadFile(std::string filename) {
	pugi::xml_parse_result result = doc.load_file(filename.c_str(), pugi::parse_default | pugi::parse_declaration);
	if (result)
		return true;
	std::cout << "cannot open file\n";
	return false;
}

void XMLManager::GenerateRoom(std::string filename, std::map<std::string, Room*>& rooms) {
	if (LoadFile(filename)) {
		pugi::xml_node xmlRooms = doc.child("level");

		// generate all the rooms in that level
		for (pugi::xml_node_iterator room = xmlRooms.begin(); room != xmlRooms.end(); room++) {
			Room* newRoom = new Room();
			newRoom->name = room->name();

			// player walking limit
			glm::vec3 minLimit = glm::vec3(room->attribute("left_limit").as_int(), room->attribute("bottom_limit").as_int(), 1);
			glm::vec3 maxLimit = glm::vec3(room->attribute("right_limit").as_int(), room->attribute("top_limit").as_int(), 1);
			newRoom->SetPlayerWalkLimit(new Collider(minLimit, maxLimit));

			std::vector<DrawableObject*>& objects = newRoom->objects;
			GenerateImage(*room, objects, "background");
			GenerateInteractObj(*room, objects);
			GenerateDoor(*room, objects, newRoom->doors);
			GenerateItem(*room, objects);
			GenerateNPC(*room, objects);
			GenerateImage(*room, objects, "foreground");

			// camera limit
			newRoom->SetCameraLimit(new Collider(objects[0]));

			rooms.insert(std::pair<std::string, Room*>(newRoom->name, newRoom));
		}
	}
}

void XMLManager::GenerateImage(pugi::xml_node room, std::vector<DrawableObject*>& objects, std::string type) {
	pugi::xml_node background = room.child(type.c_str());

	for (pugi::xml_node_iterator child = background.begin(); child != background.end(); child++) {
		//std::cout << "in GenerateBackground\n";
		ImageObject* bg = new ImageObject();
		CreateObject(bg, *child);
		objects.push_back(bg);
	}
}

void XMLManager::GenerateInteractObj(pugi::xml_node room, std::vector<DrawableObject*>& objects) {
	pugi::xml_node interactObj = room.child("interactObj");

	for (pugi::xml_node_iterator child = interactObj.begin(); child != interactObj.end(); child++) {
		//std::cout << "in GenerateInteractObj\n";
		InteractableObj* interactObj = new InteractableObj();
		interactObj->object_name = child->name();
		CreateObject(interactObj, *child);

		std::vector<std::string> dialogues;

		if (pugi::xml_node dialogue = child->child("dialogue")) {
			for (pugi::xml_node_iterator d = dialogue.begin(); d != dialogue.end(); d++) {
				dialogues.push_back(d->child_value());
			}
		}
		interactObj->SetDialogue(dialogues);
		interactObj->SetCollder(new Collider(interactObj));

		objects.push_back(interactObj);
	}
}

void XMLManager::GenerateDoor(pugi::xml_node room, std::vector<DrawableObject*>& objects, std::map<std::string, Door*>& doorsList) {
	pugi::xml_node doors = room.child("doors");

	for (pugi::xml_node_iterator child = doors.begin(); child != doors.end(); child++) {
		//std::cout << "in GenerateDoor\n";
		std::string next_room = child->attribute("next_room").as_string();
		std::string next_door = child->attribute("next_door").as_string();

		Door* door = new Door(next_room, next_door);
		door->object_name = child->name();

		CreateObject(door, *child);
		std::vector<std::string> dialogues;

		if (pugi::xml_node dialogue = child->child("dialogue")) {
			for (pugi::xml_node_iterator d = dialogue.begin(); d != dialogue.end(); d++) {
				dialogues.push_back(d->child_value());
			}
		}

		door->SetDialogue(dialogues);
		door->SetCollder(new Collider(door));

		if (child->child("key")) {
			door->SetKey(child->child("key").attribute("name").as_string());
		}

		objects.push_back(door);
		doorsList.insert(std::pair<std::string, Door*>(child->name(), door));
	}
}

void XMLManager::GenerateItem(pugi::xml_node room, std::vector<DrawableObject*>& objects) {
	pugi::xml_node items = room.child("item");

	for (pugi::xml_node_iterator child = items.begin(); child != items.end(); child++) {
		Item* item;
		//std::cout << "in GenerateItem\n";
		std::vector<Item*> resultItems;
		// if the item can be separate or combine
		if (child->first_child()) {
			for (pugi::xml_node_iterator cChild = child->begin(); cChild != child->end(); cChild++) {
				Item* tmp = new Item(cChild->name());
				tmp->SetInventoryTexture(cChild->attribute("i_texture").as_string());
				resultItems.push_back(tmp);
			}
		}

		switch (child->attribute("type").as_int())
		{
		case SEPARATABLE:
			item = new SeparatableItem(resultItems);
			break;
		case COMBINABLE:
			item = new CombinableItem(resultItems[0]->object_name, resultItems[1]);
			break;
		default:
			item = new Item();
			break;
		}

		item->SetName(child->name());
		std::string i_texture = child->attribute("i_texture").as_string();
		item->SetInventoryTexture(i_texture);

		CreateObject(item, *child);
		item->SetCollder(new Collider(item));
		objects.push_back(item);
	}
}

void XMLManager::GenerateNPC(pugi::xml_node room, std::vector<DrawableObject*>& objects) {

}

void XMLManager::GeneratePuzzle(pugi::xml_node room, std::vector<DrawableObject*>& objects) {

}

void XMLManager::CreateObject(ImageObject* tmp, pugi::xml_node node) {
	//std::cout << "in CreateObject\n";

	std::string texture = node.attribute("texture").as_string();
	int sizeX = node.attribute("sizeX").as_int();
	int sizeY = node.attribute("sizeY").as_int();
	int posX = node.attribute("posX").as_float();
	int posY = node.attribute("posY").as_float();

	tmp->SetTexture(texture);
	tmp->SetSize(sizeX, -sizeY);
	tmp->SetPosition(glm::vec3(posX, posY, 1.0));
}

int XMLManager::GetLevelNumber(std::string filename) {
	if (LoadFile(filename))
		return doc.child("level").attribute("currentLevel").as_int();
}

void XMLManager::LoadFromSave(std::string filename) {
	if (LoadFile(filename)) {
		Game* game = Game::GetInstance();
		GameScreen* gs = (GameScreen*)game->GetScreen();
		Level* level = gs->GetCurrentLevel();

		// check saved level
		// if not the same level, reload the saved level
		if (int l = doc.child("level").attribute("currentLevel").as_int() != level->levelNo)
			gs->ChangeLevel(l - 1);
		std::map<std::string, Room*>::iterator itr;
		for (itr = level->rooms.begin(); itr != level->rooms.end(); itr++) {
			std::vector<DrawableObject*> objects = itr->second->objects;
			for (int i = 0; i < itr->second->objects.size(); i++) {
				// load from save
				if (Item * item = dynamic_cast<Item*>(objects[i])) {
					item->SetDisplay(doc.child("level").child("item").child(item->object_name.c_str()).attribute("display").as_bool());
					item->SetCurrentDialogue(doc.child("level").child("item").child(item->object_name.c_str()).attribute("current_dialogue").as_int());
				}
				else if (Door * door = dynamic_cast<Door*>(objects[i])) {
					door->lock = doc.child("level").child("doors").child(door->object_name.c_str()).attribute("lock").as_bool();
					door->SetCurrentDialogue(doc.child("level").child("doors").child(door->object_name.c_str()).attribute("current_dialogue").as_int());
				}
				else if (InteractableObj * obj = dynamic_cast<InteractableObj*>(objects[i])) {
					obj->SetCurrentDialogue(doc.child("level").child("interactObj").child(obj->object_name.c_str()).attribute("current_dialogue").as_int());
				}
				/*else if (dynamic_cast<NonPlayer*>(objects[i])) {

				}*/
			}
		}

		level->ChangeRoom(doc.child("level").attribute("currentRoom").as_string());

		// load puzzles


		// load player and inventory
		pugi::xml_node playerNode = doc.child("level").child("Player");
		Player* player = game->GetPlayer();
		float px = playerNode.attribute("posX").as_int();
		float py = playerNode.attribute("posY").as_int();
		glm::vec3 playerPos(px, py, 1.0f);
		player->SetPosition(playerPos);
		player->next_position = playerPos;

		pugi::xml_node item = playerNode.child("inventory").first_child();
		for (int i = 0; i < player->inventory->GetSize(); i++) {
			// has attribute named "name"
			if (item.attribute("name").as_string() != "") {
				// put that item in the inventory
				player->inventory->GetInventoryBox(i)->SetItem((Item*)level->FindObject(item.attribute("name").as_string()));
			}
			item = item.next_sibling();
		}

		// load infoPhone
	}
}

void XMLManager::SaveGame(std::string filename) {
	Game* game = Game::GetInstance();
	pugi::xml_document save;
	Level* level = game->GetCurrentLevel();
	save.append_child("level").append_attribute("currentLevel").set_value(level->levelNo);
	save.child("level").append_attribute("currentRoom").set_value(level->GetCurrentRoom()->name.c_str());
	save.child("level").append_child("interactObj");
	save.child("level").append_child("item");
	save.child("level").append_child("doors");
	save.child("level").append_child("NPCs");
	save.child("level").append_child("puzzles");
	save.child("level").append_child("Player");
	save.child("level").child("Player").append_child("inventory");

	pugi::xml_node saveLevel = save.child("level");

	// saving objects
	std::map<std::string, Room*>::iterator itr;
	for (itr = level->rooms.begin(); itr != level->rooms.end(); itr++) {
		std::vector<DrawableObject*> objects = itr->second->objects;
		for (int i = 0; i < objects.size(); i++) {
			// if object is an interactObj, NPC, door or items 
			// save

			if (Item * item = dynamic_cast<Item*>(objects[i])) {
				saveLevel.child("item").append_child(item->object_name.c_str());
				// display
				saveLevel.child("item").child(item->object_name.c_str()).append_attribute("display").set_value(item->IsDisplay());
				// current dialogue
				saveLevel.child("item").child(item->object_name.c_str()).append_attribute("current_dialogue").set_value(item->GetCurrentDialogue());
			}
			else if (Door * door = dynamic_cast<Door*>(objects[i])) {
				saveLevel.child("doors").append_child(objects[i]->object_name.c_str()).append_attribute("lock").set_value(door->lock);
				saveLevel.child("doors").child(door->object_name.c_str()).append_attribute("current_dialogue").set_value(door->GetCurrentDialogue());
			}
			else if (InteractableObj * obj = dynamic_cast<InteractableObj*>(objects[i])) {
				saveLevel.child("interactObj").append_child(obj->object_name.c_str());
				// current dialogue
				saveLevel.child("interactObj").child(obj->object_name.c_str()).append_attribute("current_dialogue").set_value(obj->GetCurrentDialogue());
			}
			/*else if (dynamic_cast<NonPlayer*>(objects[i])) {

			}*/
		}
	}

	// saving puzzles 


	// saving player and inventory
	Player* player = game->GetPlayer();
	saveLevel.child("Player").append_attribute("posX").set_value(player->getPos().x);
	saveLevel.child("Player").append_attribute("posY").set_value(player->getPos().y);

	for (int i = 0; i < player->inventory->GetSize(); i++) {
		pugi::xml_node node = saveLevel.child("Player").child("inventory").append_child("item");
		Item* item = player->inventory->GetInventoryBox(i)->GetItem();
		if (item != nullptr)
			node.append_attribute("name").set_value(item->object_name.c_str());
	}

	// save infoPhone

	save.save_file(filename.c_str());
}

void XMLManager::GetChat(std::string name, ImageObject* obj) {
	obj->SetTexture(chatDoc.child("chats").child(name.c_str()).attribute("pic").as_string());
}

std::string XMLManager::GetMessage(std::string name, int index) {
	pugi::xml_node_iterator itr = chatDoc.child("chats").child(name.c_str()).begin();
	for (int i = 0; i < index; i++) {
		itr++;
	}
	return itr->attribute("value").as_string();
}

XMLManager::~XMLManager() {
	delete _instance;
}