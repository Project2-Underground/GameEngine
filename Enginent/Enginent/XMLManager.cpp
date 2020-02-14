#include "XMLManager.h"
#include "Item.h"
#include "Game.h"

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
			GenerateImage(*room, newRoom, "background");
			GenerateInteractObj(*room, newRoom);
			GenerateDoor(*room, newRoom);
			//GenerateItem(*room, newRoom);
			GenerateNPC(*room, newRoom);
			GenerateImage(*room, newRoom, "foreground");

			// camera limit
			newRoom->SetCameraLimit(new Collider(objects[0]));
			newRoom->SortObjLayer();

			rooms.insert(std::pair<std::string, Room*>(newRoom->name, newRoom));
		}
	}
}

void XMLManager::GenerateImage(pugi::xml_node room, Room* r, std::string type) {
	pugi::xml_node background = room.child(type.c_str());

	for (pugi::xml_node_iterator child = background.begin(); child != background.end(); child++) {
		//std::cout << "in GenerateBackground\n";
		ImageObject* bg = new ImageObject();
		CreateObject(bg, *child);
		bg->subLayer = child->attribute("layer").as_int();

		if (type == "background") {
			bg->layer = BACKGROUND_LAYER;
			r->objects.push_back(bg);
		}
		else if (type == "foreground") {
			bg->layer = FOREGROUND_LAYER;
			r->foreground.push_back(bg);
		}

	}
}

void XMLManager::GenerateInteractObj(pugi::xml_node room, Room* r) {
	pugi::xml_node interactObj = room.child("interactObj");

	for (pugi::xml_node_iterator child = interactObj.begin(); child != interactObj.end(); child++) {
		//std::cout << "in GenerateInteractObj\n";
		InteractableObj* interactObj;
		InteractTypeList type = (InteractTypeList)child->attribute("type").as_int();

		switch (type)
		{
		case VIEW: {
			ViewObj* obj = new ViewObj();
			obj->SetViewTexture(child->child("view").attribute("texture").as_string());
			// set description
			interactObj = obj;
		}break;
		case OPEN: {
			OpenObj* obj = new OpenObj();
			obj->SetOpenTexture(child->child("clicked").attribute("texture").as_string());
			if (child->child("item")) {
				obj->SetNextTexture(child->child("item").attribute("texture").as_string());
				Item* item = new Item(child->child("item").attribute("name").as_string());
				item->SetInventoryTexture(child->child("item").attribute("i_texture").as_string());
				obj->SetItem(item);
			}
			interactObj = obj;
		}break;
		case PUZZLE: {
			PuzzleObj* obj = new PuzzleObj();
			obj->SetPuzzleName(child->child("puzzle").attribute("name").as_string());

			interactObj = obj;
		}break;
		default: {
			interactObj = new InteractableObj();
		}break;
		}

		if (child->child("picture"))
			interactObj->SetTakePic(child->child("picture").attribute("name").as_string());


		interactObj->object_name = child->name();
		CreateObject(interactObj, *child);

		std::vector<Dialogue> dialogues;
		pugi::xml_node_iterator n;

		if (pugi::xml_node dialogue = child->child("dialogue")) {
			for (pugi::xml_node_iterator d = dialogue.begin(); d != dialogue.end(); d++) {
				n = d;
				d++;
				dialogues.push_back(Dialogue(n->child_value(), d->child_value()));
			}
		}

		if (child->child("key"))
			interactObj->SetItemToUse(child->child("key").attribute("name").as_string());

		interactObj->SetDialogue(dialogues);
		interactObj->SetCollder(new Collider(interactObj));
		interactObj->layer = OBJECT_LAYER;
		interactObj->subLayer = child->attribute("layer").as_int();

		r->objects.push_back(interactObj);
	}
}

void XMLManager::GenerateDoor(pugi::xml_node room, Room* r) {
	pugi::xml_node doors = room.child("doors");

	for (pugi::xml_node_iterator child = doors.begin(); child != doors.end(); child++) {
		//std::cout << "in GenerateDoor\n";
		std::string next_room = child->attribute("next_room").as_string();
		std::string next_door = child->attribute("next_door").as_string();

		Door* door = new Door(next_room, next_door);
		door->object_name = child->name();

		CreateObject(door, *child);
		std::vector<Dialogue> dialogues;
		pugi::xml_node_iterator n;

		if (pugi::xml_node dialogue = child->child("dialogue")) {
			for (pugi::xml_node_iterator d = dialogue.begin(); d != dialogue.end(); d++) {
				n = d;
				d++;
				dialogues.push_back(Dialogue(n->child_value(), d->child_value()));
			}
		}

		door->SetDialogue(dialogues);
		door->SetCollder(new Collider(door));

		if (child->child("key")) {
			door->SetItemToUse(child->child("key").attribute("name").as_string());
		}

		door->layer = OBJECT_LAYER;
		door->subLayer = child->attribute("layer").as_int();

		r->objects.push_back(door);
		r->doors.insert(std::pair<std::string, Door*>(child->name(), door));
	}
}

//void XMLManager::GenerateItem(pugi::xml_node room, Room* r) {
//	pugi::xml_node items = room.child("item");
//
//	for (pugi::xml_node_iterator child = items.begin(); child != items.end(); child++) {
//		Item* item;
//		//std::cout << "in GenerateItem\n";
//		std::vector<Item*> resultItems;
//		// if the item can be separate or combine
//		if (child->first_child()) {
//			for (pugi::xml_node_iterator cChild = child->begin(); cChild != child->end(); cChild++) {
//				Item* tmp = new Item(cChild->name());
//				tmp->SetInventoryTexture(cChild->attribute("i_texture").as_string());
//				resultItems.push_back(tmp);
//			}
//		}
//
//		std::string itemName = child->name();
//
//		switch (child->attribute("type").as_int())
//		{
//		case SEPARATABLE:
//			item = new SeparatableItem(itemName, resultItems);
//			break;
//		case COMBINABLE:
//			item = new CombinableItem(itemName, resultItems[0]->name, resultItems[1]);
//			break;
//		default:
//			item = new Item(itemName);
//			break;
//		}
//
//		std::string i_texture = child->attribute("i_texture").as_string();
//		item->SetInventoryTexture(i_texture);
//
//		r->items.push_back(item);
//	}
//}

void XMLManager::GenerateNPC(pugi::xml_node room, Room* r) {

}

void XMLManager::GeneratePuzzle(pugi::xml_node room, Room* r) {

}

void XMLManager::CreateObject(ImageObject* tmp, pugi::xml_node node) {
	//std::cout << "in CreateObject\n";

	std::string texture = node.attribute("texture").as_string();
	float sizeX = node.attribute("sizeX").as_float();
	float sizeY = node.attribute("sizeY").as_float();
	float posX = node.attribute("posX").as_float();
	float posY = node.attribute("posY").as_float();

	if (texture == "NONE")
		tmp->SetDisplay(false);
	else
		tmp->SetTexture(texture);
	tmp->SetSize(sizeX, -sizeY);
	tmp->SetPosition(glm::vec3(posX, posY, 1.0));
}

int XMLManager::GetLevelNumber(std::string filename) {
	if (LoadFile(filename))
		return doc.child("level").attribute("currentLevel").as_int();
	return 0;
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
				if (Door * door = dynamic_cast<Door*>(objects[i])) {
					door->used = doc.child("level").child("doors").child(door->object_name.c_str()).attribute("used").as_bool();
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
		float px = playerNode.attribute("posX").as_float();
		float py = playerNode.attribute("posY").as_float();
		glm::vec3 playerPos(px, py, 1.0f);
		player->SetPosition(playerPos);
		player->next_position = playerPos;

		pugi::xml_node item = playerNode.child("inventory").first_child();
		for (int i = 0; i < gs->GetInventory()->GetSize(); i++) {
			// has attribute named "name"
			if (item.attribute("name").as_string() != "") {
				// put that item in the inventory
				gs->GetInventory()->GetInventoryBox(i)->SetItem((Item*)level->FindObject(item.attribute("name").as_string()));
			}
			item = item.next_sibling();
		}

		// load infoPhone
		pugi::xml_node phoneNode = doc.child("level").child("Phone");
		Phone* phone = Phone::GetInstance();
		for (pugi::xml_node_iterator itr = phoneNode.child("Notes").begin(); itr != phoneNode.child("Notes").end(); itr++) {
			phone->AddPage(NOTE, itr->attribute("name").as_string());
		}
		int count = 0;
		for (pugi::xml_node_iterator itr = phoneNode.child("Chats").begin(); itr != phoneNode.child("Chats").end(); itr++) {
			phone->AddPage(CHAT, itr->attribute("name").as_string());
			phone->app->chats[count++]->currentMsgIndex = itr->attribute("index").as_int();
		}
	}
}

void XMLManager::SaveGame(std::string filename) {
	Game* game = Game::GetInstance();
	GameScreen* gs = (GameScreen*)game->GetScreen();
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
	save.child("level").append_child("Phone");
	save.child("level").child("Phone").append_child("Notes");
	save.child("level").child("Phone").append_child("Chats");;

	pugi::xml_node saveLevel = save.child("level");

	// saving objects
	std::map<std::string, Room*>::iterator itr;
	for (itr = level->rooms.begin(); itr != level->rooms.end(); itr++) {
		std::vector<DrawableObject*> objects = itr->second->objects;
		for (int i = 0; i < objects.size(); i++) {
			// if object is an interactObj, NPC, door or items 
			// save

			if (Door * door = dynamic_cast<Door*>(objects[i])) {
				saveLevel.child("doors").append_child(objects[i]->object_name.c_str()).append_attribute("used").set_value(door->used);
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

	for (int i = 0; i < gs->GetInventory()->GetSize(); i++) {
		pugi::xml_node node = saveLevel.child("Player").child("inventory").append_child("item");
		Item* item = gs->GetInventory()->GetInventoryBox(i)->GetItem();
		if (item != nullptr)
			node.append_attribute("name").set_value(item->name.c_str());
	}

	// save infoPhone
	Phone* phone = Phone::GetInstance();
	int noteSize = (int)phone->app->notes.size();
	int chatSize = (int)phone->app->chats.size();

	for (int i = 0; i < noteSize; i++) {
		pugi::xml_node node = saveLevel.child("Phone").child("Notes").append_child("n");
		node.append_attribute("name").set_value(phone->app->notes[i]->object_name.c_str());
	}

	for (int i = 0; i < chatSize; i++) {
		pugi::xml_node node = saveLevel.child("Phone").child("Chats").append_child("c");
		node.append_attribute("name").set_value(phone->app->chats[i]->name.c_str());
		node.append_attribute("index").set_value(phone->app->chats[i]->currentMsgIndex);
	}

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

void XMLManager::SaveGameOptions() {
	Game* game = Game::GetInstance();
	pugi::xml_document gameOption;

	gameOption.append_child("Options").append_child("BG_music").append_attribute("mute").set_value(game->muteBG);
	gameOption.child("Options").append_child("SFX").append_attribute("mute").set_value(game->muteSFX);
}

XMLManager::~XMLManager() {
	delete _instance;
}
