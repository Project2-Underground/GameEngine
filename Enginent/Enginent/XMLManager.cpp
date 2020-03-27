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

			//std::cout << newRoom->name << std::endl;

			// player walking limit
			glm::vec3 minLimit = glm::vec3(room->attribute("left_limit").as_int(), room->attribute("bottom_limit").as_int(), 1);
			glm::vec3 maxLimit = glm::vec3(room->attribute("right_limit").as_int(), room->attribute("top_limit").as_int(), 1);
			newRoom->SetPlayerWalkLimit(new Collider(minLimit, maxLimit));

			std::vector<DrawableObject*>& objects = newRoom->objects;
			GenerateImage(*room, newRoom, "background");
			GenerateInteractObj(*room, newRoom);
			GenerateDoor(*room, newRoom);
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
				item->SetViewTexture(child->child("item").attribute("v_texture").as_string());
				obj->SetItem(item);
			}
			interactObj = obj;
		}break;
		case PUZZLE: {
			PuzzleObj* obj = new PuzzleObj();
			obj->SetPuzzleName(child->child("puzzle").attribute("name").as_string());

			interactObj = obj;
		}break;
		case SAVE: {
			interactObj = new SaveObj();
		}break;
		default: {
			interactObj = new InteractableObj();
		}break;
		}

		if (child->child("picture"))
			interactObj->SetTakePic(child->child("picture").attribute("name").as_string());


		interactObj->object_name = child->name();
		CreateObject(interactObj, *child);

		if (child->child("key"))
			interactObj->SetItemToUse(child->child("key").attribute("name").as_string());

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

		Door* door;
		if (child->child("wall_door"))
			door = new WallDoor(next_room, next_door);
		else
			door = new Door(next_room, next_door);

		door->object_name = child->name();

		CreateObject(door, *child);

		door->SetCollder(new Collider(door));

		if (child->child("key"))
			door->SetItemToUse(child->child("key").attribute("name").as_string());

		if (child->child("openDoor"))
			door->SetOpenTexture(child->child("openDoor").attribute("texture").as_string());
		else
			door->SetOpenTexture(door->GetTexture());

		door->layer = OBJECT_LAYER;
		door->subLayer = child->attribute("layer").as_int();

		if (child->attribute("stair"))
			door->SetInteractType(STAIR);

		r->objects.push_back(door);
		r->doors.insert(std::pair<std::string, Door*>(child->name(), door));
	}
}

void XMLManager::GenerateNPC(pugi::xml_node room, Room* r) {
	pugi::xml_node npcs = room.child("NPCs");

	for (pugi::xml_node_iterator child = npcs.begin(); child != npcs.end(); child++) {
		NonPlayer* npc = new NonPlayer(child->name());
		CreateObject(npc, *child);

		if (child->child("item")) {
			Item* item = new Item(child->child("item").attribute("name").as_string());
			item->SetInventoryTexture(child->child("item").attribute("i_texture").as_string());
			item->SetViewTexture(child->child("item").attribute("v_texture").as_string());
			npc->SetItem(item);
		}

		//std::vector<Dialogue> dialogues;
		//pugi::xml_node_iterator n;

		//if (pugi::xml_node dialogue = child->child("dialogue")) {
		//	for (pugi::xml_node_iterator d = dialogue.begin(); d != dialogue.end(); d++) {
		//		n = d;
		//		d++;
		//		dialogues.push_back(Dialogue(n->child_value(), d->child_value()));
		//	}
		//}

		//npc->SetDialogue(dialogues);
		npc->SetCollder(new Collider(npc));
		npc->layer = NPC_LAYER;
		npc->subLayer = child->attribute("layer").as_int();

		r->npcs.push_back(npc);
	}
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
	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file(filename.c_str(), pugi::parse_default | pugi::parse_declaration);
	if (result) {
		Game* game = Game::GetInstance();
		GameScreen* gs = (GameScreen*)game->GetScreen();

		int l = file.child("level").attribute("currentLevel").as_int();
		gs->ChangeLevel(l - 1);

		Level* level = gs->GetCurrentLevel();

		std::map<std::string, Room*>::iterator itr;
		for (itr = level->rooms.begin(); itr != level->rooms.end(); itr++) {
			std::vector<DrawableObject*> objects = itr->second->objects;
			for (int i = 0; i < objects.size(); i++) {
				// load from save
				if (Door * door = dynamic_cast<Door*>(objects[i])) {
					if (file.child("level").child("doors").child(door->object_name.c_str()).attribute("used").as_bool())
						door->Open();
				}
				else if (InteractableObj * obj = dynamic_cast<InteractableObj*>(objects[i])) {
					pugi::xml_node node = file.child("level").child("interactObj").child(obj->object_name.c_str());

					if (OpenObj * o = dynamic_cast<OpenObj*>(obj)) {
						if (node.attribute("open").as_bool()) {
							o->Open();
							if (!node.attribute("has_item").as_bool())
								o->ClearItem();
						}
					}
				}
			}
		}


		level->ChangeRoom(file.child("level").attribute("currentRoom").as_string());

		// load puzzles
		pugi::xml_node puzzles = file.child("level").child("puzzles");
		pugi::xml_node_iterator p;
		for (p = puzzles.begin(); p != puzzles.end(); p++) {
			if (p->attribute("done").as_bool())
				gs->puzzles[p->name()]->CompletePuzzle();
		}

		// load player and inventory
		pugi::xml_node playerNode = file.child("level").child("Player");
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
		pugi::xml_node phoneNode = file.child("level").child("Phone");
		Phone* phone = Phone::GetInstance();
		phone->Clear();
		for (pugi::xml_node_iterator itr = phoneNode.child("Notes").begin(); itr != phoneNode.child("Notes").end(); itr++) {
			phone->SetPage(NOTE, itr->attribute("name").as_string());
		}

		for (pugi::xml_node_iterator itr = phoneNode.child("Chats").begin(); itr != phoneNode.child("Chats").end(); itr++) {
			std::string name = itr->attribute("name").as_string();
			phone->SetPage(CHAT, name);
			phone->chats[name].currentMsgIndex = itr->attribute("msgNo").as_int();
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
				//saveLevel.child("doors").child(door->object_name.c_str()).append_attribute("current_dialogue").set_value(door->GetCurrentDialogue());
			}
			else if (InteractableObj * obj = dynamic_cast<InteractableObj*>(objects[i])) {
				saveLevel.child("interactObj").append_child(obj->object_name.c_str());
				pugi::xml_node node = saveLevel.child("interactObj").child(obj->object_name.c_str());
				/// current dialogue
				//node.append_attribute("current_dialogue").set_value(obj->GetCurrentDialogue());
				
				if (OpenObj * o = dynamic_cast<OpenObj*>(obj)) {
					node.append_attribute("open").set_value(o->IsOpen());
					bool hasItem = false;
					if (o->GetItem())
						hasItem = true;
					node.append_attribute("has_item").set_value(hasItem);
				}
			}
		}
	}

	// saving puzzles 
	for (auto p : gs->puzzles) {
		saveLevel.child("puzzles").append_child(p.first.c_str());
		saveLevel.child("puzzles").child(p.first.c_str()).append_attribute("done").set_value(p.second->Passed());
	}

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
		node.append_attribute("name").set_value(phone->app->notes[i]->name.c_str());
	}

	for (auto c:phone->chats) {
		pugi::xml_node node = saveLevel.child("Phone").child("Chats").append_child("c");
		node.append_attribute("name").set_value(c.second.name.c_str());
		node.append_attribute("msgNo").set_value(c.second.texts.size());
	}

	save.save_file(filename.c_str());
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

	SoundManager* soundManager = SoundManager::GetInstance();

	gameOption.append_child("Options");
	gameOption.child("Options").append_child("Master").append_attribute("mute").set_value(soundManager->getMute(MASTER));
	gameOption.child("Options").child("Master").append_attribute("volume").set_value(soundManager->getVolume(MASTER));

	gameOption.child("Options").append_child("BGM").append_attribute("mute").set_value(soundManager->getMute(BGM));
	gameOption.child("Options").child("BGM").append_attribute("volume").set_value(soundManager->getVolume(BGM));

	gameOption.child("Options").append_child("SFX").append_attribute("mute").set_value(soundManager->getMute(SFX));
	gameOption.child("Options").child("SFX").append_attribute("volume").set_value(soundManager->getVolume(SFX));

	gameOption.save_file("save/settings.xml");
}

void XMLManager::LoadGameOptions() {
	std::cout << "load game options\n";
	if (LoadFile("save/settings.xml")) {
		SoundManager* soundManager = SoundManager::GetInstance();

		pugi::xml_node options = doc.child("Options");

		soundManager->setVolume(MASTER, options.child("Master").attribute("volume").as_float());
		soundManager->setVolume(BGM, options.child("BGM").attribute("volume").as_float());
		soundManager->setVolume(SFX, options.child("SFX").attribute("volume").as_float());

		soundManager->setMute(MASTER, options.child("Master").attribute("mute").as_bool());
		soundManager->setMute(BGM, options.child("BGM").attribute("mute").as_bool());
		soundManager->setMute(SFX, options.child("SFX").attribute("mute").as_bool());
	}
}

XMLManager::~XMLManager() {

}

void XMLManager::LoadNotes(std::string filename, std::map<std::string, UIObject*>& notes) {
	if (LoadFile(filename)) {
		pugi::xml_node allNotes = doc.child("notes");

		// generate all the rooms in that level
		for (pugi::xml_node_iterator note = allNotes.begin(); note != allNotes.end(); note++) {
			UIObject* tmp = new UIObject();
			tmp->SetTexture(note->attribute("texture").as_string());
			tmp->SetName(note->name());
			notes.insert(std::pair<std::string, UIObject*>(tmp->object_name, tmp));
		}
	}
}

void XMLManager::LoadChats(std::string filename, std::map<std::string, ChatInfo>& chats) {
	if (LoadFile(filename)) {
		pugi::xml_node allChats = doc.child("chats");

		for (pugi::xml_node_iterator chat = allChats.begin(); chat != allChats.end(); chat++) {
			ChatInfo c;
			c.name = chat->name();
			c.picTexture = Game::GetInstance()->GetRenderer()->LoadTexture(chat->attribute("pic").as_string());
			if (chat->first_child()) 
				for (pugi::xml_node_iterator msg = chat->begin(); msg != chat->end(); msg++) {
					c.AddText(msg->child_value());
				}
			chats.insert(std::pair<std::string, ChatInfo>(c.name, c));
		}
	}
}