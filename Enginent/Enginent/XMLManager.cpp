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
	std::cout << "cannot open file " << filename << std::endl;
	return false;
}

std::string XMLManager::GetFirstRoomName() {
	return doc.child("level").attribute("Start").as_string();
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
			GenerateDoor(*room, newRoom);
			GenerateNPC(*room, newRoom);
			GenerateImage(*room, newRoom, "foreground");
			GenerateInteractObj(*room, newRoom);

			// camera limit
			newRoom->SetCameraLimit(new Collider(objects[0]));
			newRoom->SortObjLayer();
			if(room->attribute("dialogue"))
				newRoom->dialogue = room->attribute("dialogue").as_string();
			newRoom->y = room->attribute("y").as_float();

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
			obj->viewWidth = (child->child("view").attribute("sizeX").as_float());
			obj->viewHeight = (child->child("view").attribute("sizeY").as_float());
			interactObj = obj;
		}break;
		case OPEN: {
			OpenObj* obj = new OpenObj();
			obj->SetOpenTexture(child->child("clicked").attribute("texture").as_string());
			interactObj = obj;
		}break;
		case PUZZLE: {
			PuzzleObj* obj = new PuzzleObj();
			obj->SetPuzzleName(child->child("puzzle").attribute("name").as_string());
			//std::cout << "puzzle created " << child->name() << std::endl;
			interactObj = obj;
		}break;
		case SAVE: {
			interactObj = new SaveObj();
		}break;
		case PLAYER_TRIGGER: {
			PlayerTriggerObj* obj = new PlayerTriggerObj();
			obj->SetInteractType(PLAYER_TRIGGER);
			//std::cout << "set trigger name: " << child->name() << "\n";
			interactObj = obj;
		}break;
		case ADDNOTE: {
			InteractableObj* obj = new InteractableObj();
			obj->SetInteractType(ADDNOTE);
			interactObj = obj;
		}break;
		case PICKUP: {
			interactObj = new InteractableObj();
			interactObj->SetInteractType(PICKUP);
		}break;
		default: {
			interactObj = new InteractableObj();
		}break;
		}
		interactObj->object_name = child->name();
		CreateObject(interactObj, *child);

		pugi::xml_node animaions = child->child("Animations");
		if (animaions)
			interactObj->InitAnimator();
		for (pugi::xml_node_iterator anim = animaions.begin(); anim != animaions.end(); anim++) {
			interactObj->anim->AddAnimation(anim->name(),
				anim->attribute("texture").as_string(),
				anim->attribute("frame").as_int(),
				anim->attribute("time_per_frame").as_float(),
				anim->attribute("loop").as_bool());
		}
		if (child->child("typeView")) {
			interactObj->SetInteractType((InteractTypeList)child->child("typeView").attribute("type").as_int());
		}

		if (child->child("sound"))
			interactObj->SetSound(child->child("sound").attribute("name").as_string());

		if (child->child("item")) {
			interactObj->SetItem(child->child("item").attribute("name").as_string());
			interactObj->scriptHandleItem = child->child("item").attribute("scriptHandleItem").as_bool();
			//std::cout << "XMLManager::GenerateInteractObj scriptHandleItem "<< child->name() << " : " << interactObj->scriptHandleItem << std::endl;
		}

		if (child->child("picked"))
			interactObj->SetNextTexture(child->child("picked").attribute("texture").as_string());

		if (child->child("key"))
			interactObj->SetItemToUse(child->child("key").attribute("name").as_string());

		if (child->child("used")) {
			interactObj->SetUsedTexture(child->child("used").attribute("texture").as_string());
			if (child->child("used").child("positionAfterUse")) {
				interactObj->SetPositionAfterUsed(child->child("used").child("positionAfterUse").attribute("x").as_float(), 
												  child->child("used").child("positionAfterUse").attribute("y").as_float());
			}
			if (child->child("used").child("newItemAfterUsed")) {
				interactObj->SetNewItemAfterUsed(child->child("used").child("newItemAfterUsed").attribute("name").as_string());
			}
		}

		if (child->child("note"))
			for(pugi::xml_node_iterator note = child->child("note").begin(); note != child->child("note").end(); note++)
				interactObj->AddNoteName(note->name());

		interactObj->SetCollder(new Collider(interactObj));
		if (child->child("colOff")) {
			interactObj->col->enable = false;
		}

		interactObj->layer = OBJECT_LAYER;
		interactObj->subLayer = child->attribute("layer").as_int();
		interactObj->SetDialogueName(child->attribute("dialogue").as_string(), child->attribute("dialogue_2").as_string());

		if(child->child("ForeGround"))
			r->foreground.push_back(interactObj);
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
		if (child->child("wall_door")) {
			door = new WallDoor(next_room, next_door);
			door->SetDialogueAfterTriggerName(child->attribute("dialogueAfterTrigger").as_string());
		}
		else if (child->child("SecretDoor"))
			door = new SecretDoor(next_room, next_door);
		else
			door = new Door(next_room, next_door);

		door->object_name = child->name();

		if (child->child("isOpenDoor"))
			door->isOpenDoor = true;

		CreateObject(door, *child);

		door->SetCollder(new Collider(door));

		if (child->child("playerNextX"))
			door->SetPlayerNextX(child->child("playerNextX").attribute("value").as_float());
		
		if (child->child("key"))
			door->SetItemToUse(child->child("key").attribute("name").as_string());

		if (child->child("openDoor"))
			door->SetOpenTexture(child->child("openDoor").attribute("texture").as_string());

		door->layer = OBJECT_LAYER;
		door->subLayer = child->attribute("layer").as_int();
		door->SetDialogueName(child->attribute("dialogue").as_string(), child->attribute("dialogue_2").as_string());

		if (child->child("colOff")) {
			door->col->enable = false;
		}

		if (child->attribute("stair")) 
			door->SetInteractType(STAIR);

		if (child->child("ForeGround"))
			r->foreground.push_back(door);
		r->objects.push_back(door);
		r->doors.insert(std::pair<std::string, Door*>(child->name(), door));
	}
}

void XMLManager::GenerateNPC(pugi::xml_node room, Room* r) {
	pugi::xml_node npcs = room.child("NPCs");

	for (pugi::xml_node_iterator child = npcs.begin(); child != npcs.end(); child++) {
		NonPlayer* npc;
		if (child->name() == "BackAlley_Emma") {
			npc = new BackAlleyEmma(child->name());
		}
		else {
			npc = new NonPlayer(child->name());
		}

		float sizeX = child->attribute("sizeX").as_float();
		float sizeY = child->attribute("sizeY").as_float();
		float posX = child->attribute("posX").as_float();
		float posY = child->attribute("posY").as_float();
		npc->Init(sizeX, -sizeY, glm::vec3(posX, posY, 1.0));

		if (child->child("item")) 
			npc->SetItem(child->child("item").attribute("name").as_string());

		if (child->child("colOff")) {
			npc->col->enable = false;
			npc->SetDisplay(false);
		}
		if (child->child("key"))
			npc->SetItemToUse(child->child("key").attribute("name").as_string());

		pugi::xml_node animaions = child->child("Animations");
		if(animaions)
			npc->InitAnimator();
		for (pugi::xml_node_iterator anim = animaions.begin(); anim != animaions.end(); anim++) {
			npc->anim->AddAnimation(anim->name(), 
									anim->attribute("texture").as_string(), 
									anim->attribute("frame").as_int(), 
									anim->attribute("time_per_frame").as_float(), 
									anim->attribute("loop").as_bool());
		}

		npc->layer = NPC_LAYER;
		npc->subLayer = child->attribute("layer").as_int();
		npc->SetDialogueName(child->attribute("dialogue").as_string(), child->attribute("dialogue_2").as_string());

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

void XMLManager::GetLevelNumber(std::string filename, Level* currentLevel) {
	if (LoadFile(filename)) {
		currentLevel->levelNo = doc.child("level").attribute("currentLevel").as_int();
		currentLevel->xStart = doc.child("level").attribute("startPosX").as_float();
	}
}

void XMLManager::LoadFromSave(std::string filename) {
	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file(filename.c_str(), pugi::parse_default | pugi::parse_declaration);
	if (result) {
		SoundManager::GetInstance()->LoadMute();
		Game* game = Game::GetInstance();
		GameScreen* gs = (GameScreen*)game->GetScreen();

		int l = file.child("level").attribute("currentLevel").as_int();
		gs->ChangeLevel(l - 1);

		Level* level = gs->GetCurrentLevel();

		level->ChangeRoom(file.child("level").attribute("currentRoom").as_string());

		// load puzzles
		pugi::xml_node puzzles = file.child("level").child("puzzles");
		pugi::xml_node_iterator p;
		for (p = puzzles.begin(); p != puzzles.end(); p++) {
			if (gs->puzzles[p->name()]->GetPuzzleLevel() == l) {
				gs->puzzles[p->name()]->Reset();
				if (p->attribute("done").as_bool())
					gs->puzzles[p->name()]->CompletePuzzle();
				gs->puzzles[p->name()]->passedReqiurements = p->attribute("passRequirements").as_bool();
			}
		}

		std::map<std::string, Room*>::iterator itr;
		for (itr = level->rooms.begin(); itr != level->rooms.end(); itr++) {
			std::vector<DrawableObject*> objects = itr->second->objects;
			std::vector<DrawableObject*> npcs = itr->second->npcs;
			itr->second->dialogue = file.child("level").child("rooms").child(itr->first.c_str()).attribute("dialogue").as_string();
			for (int i = 0; i < objects.size(); i++) {
				// load from save
				if (Door * door = dynamic_cast<Door*>(objects[i])) {
					if (file.child("level").child("doors").child(door->object_name.c_str()).attribute("used").as_bool())
						door->Open();
				}
				else if (InteractableObj * obj = dynamic_cast<InteractableObj*>(objects[i])) {
					pugi::xml_node node = file.child("level").child("interactObj").child(obj->object_name.c_str());
					obj->SetCurrentDialogueName(node.attribute("current_dialogue").as_string());
					obj->SetDialogueBeforeName(node.attribute("before_dialogue").as_string());
					obj->SetDialogueAfterName(node.attribute("after_dialogue").as_string());
					obj->SetTakeNote(node.attribute("takeNote").as_bool());
					if (node.attribute("talked").as_bool()) 
						obj->SetTalked(node.attribute("talked").as_bool());

					obj->hasItem = node.attribute("has_item").as_bool();
					if (OpenObj * o = dynamic_cast<OpenObj*>(obj)) {
						if (node.attribute("open").as_bool()) {
							o->Open();
						}
					}

					if (NumpadPuzzleAfter * n = dynamic_cast<NumpadPuzzleAfter*>(obj)) {
						if (node.attribute("used").as_bool())
							n->UnlockBookshelf();
					}

					if (RemoveObj * n = dynamic_cast<RemoveObj*>(obj)) {
						if (node.attribute("triggered").as_bool())
							n->Trigger();
						if (node.attribute("used").as_bool())
							n->RemoveSelf();
					}

					if (PlayerTriggerObj * t = dynamic_cast<PlayerTriggerObj*>(obj)) {
						t->triggered = node.attribute("triggered").as_bool();
					}
				}
			}
			for (int i = 0; i < npcs.size(); i++) {
				NonPlayer* npc = (NonPlayer*)npcs[i];
				npc->hasItem = file.child("level").child("NPCs").child(npc->object_name.c_str()).attribute("hasItem").as_bool();
			}
		}

		// load special npcs
		pugi::xml_node butlerNode = file.child("level").child("Butler");
		Butler* butler = ((GameScreen*)game->GetScreen())->butler;
		butler->currentPhase = (Butler::Phase)butlerNode.attribute("phase").as_int();
		if (butlerNode.attribute("display").as_bool())
			butler->Appear();
		else {
			butler->SetDisplay(false);
			butler->col->enable = false;
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
				Item* tmpI = gs->FindItem(item.attribute("name").as_string());
				tmpI->multipleUse = item.attribute("multipleUse").as_bool();
				gs->GetInventory()->GetInventoryBox(i)->SetItem(tmpI);
			}
			item = item.next_sibling();
		}

		// load infoPhone
		pugi::xml_node phoneNode = file.child("level").child("Phone");
		Phone* phone = Phone::GetInstance();
		phone->Clear();
		phone->firstClose = phoneNode.attribute("firstClose").as_bool();
		phone->note23Done = phoneNode.attribute("note23Track").as_bool();
		for (pugi::xml_node_iterator track = phoneNode.child("npcTalked").begin(); track != phoneNode.child("npcTalked").end(); track++) {
			phone->npcTalked.push_back(track->name());
		}
		if (phoneNode.attribute("hasPhone").as_bool())
			gs->phoneIcon->Appear();
		for (pugi::xml_node_iterator itr = phoneNode.child("Notes").begin(); itr != phoneNode.child("Notes").end(); itr++) {
			phone->SetPage(NOTE, itr->attribute("name").as_string());
		}

		for (pugi::xml_node_iterator itr = phoneNode.child("Chats").begin(); itr != phoneNode.child("Chats").end(); itr++) {
			std::string name = itr->attribute("name").as_string();
			phone->SetPage(CHAT, name);
			phone->chats[name].currentMsgIndex = itr->attribute("msgNo").as_int();
		}

		TextBox::GetInstance()->SetDisplay(false);
		SoundManager::GetInstance()->LoadMute();
	}
}

void XMLManager::SaveGame(std::string filename) {
	Game* game = Game::GetInstance();
	GameScreen* gs = (GameScreen*)game->GetScreen();
	pugi::xml_document save;
	Level* level = game->GetCurrentLevel();
	save.append_child("level").append_attribute("currentLevel").set_value(level->levelNo);
	save.child("level").append_attribute("currentRoom").set_value(level->GetCurrentRoom()->name.c_str());
	save.child("level").append_child("rooms");
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
		std::vector<DrawableObject*> npcs = itr->second->npcs;
		save.child("level").child("rooms").append_child(itr->first.c_str()).append_attribute("dialogue").set_value(itr->second->dialogue.c_str());
		for (int i = 0; i < objects.size(); i++) {
			// if object is an interactObj, NPC, door or items 
			// save

			if (Door * door = dynamic_cast<Door*>(objects[i])) {
				if(door->getType() != STAIR)
					saveLevel.child("doors").append_child(objects[i]->object_name.c_str()).append_attribute("used").set_value(door->used);
				//saveLevel.child("doors").child(door->object_name.c_str()).append_attribute("current_dialogue").set_value(door->GetCurrentDialogue());
			}
			else if (InteractableObj * obj = dynamic_cast<InteractableObj*>(objects[i])) {
				saveLevel.child("interactObj").append_child(obj->object_name.c_str());
				pugi::xml_node node = saveLevel.child("interactObj").child(obj->object_name.c_str());
				/// dialogues
				node.append_attribute("current_dialogue").set_value(obj->GetCurrentDialogueName().c_str());
				node.append_attribute("before_dialogue").set_value(obj->GetDialogueBeforeName().c_str());
				node.append_attribute("after_dialogue").set_value(obj->GetDialogueAfterName().c_str());
				node.append_attribute("talked").set_value(obj->Talked());
				node.append_attribute("takeNote").set_value(obj->TookNote());
				node.append_attribute("has_item").set_value(obj->hasItem);

				
				if (OpenObj * o = dynamic_cast<OpenObj*>(obj)) {
					node.append_attribute("open").set_value(o->IsOpen());
				}

				if (dynamic_cast<NumpadPuzzleAfter*>(obj) || dynamic_cast<RemoveObj*>(obj)) {
					node.append_attribute("used").set_value(obj->used);
				}

				if (dynamic_cast<PlayerTriggerObj*>(obj) || dynamic_cast<RemoveObj*>(obj)) {
					node.append_attribute("triggered").set_value(obj->triggered);
				}
			}
		}
		for (int i = 0; i < npcs.size(); i++) {
			NonPlayer* npc = (NonPlayer*)npcs[i];
			pugi::xml_node node = saveLevel.child("NPCs").append_child(npc->object_name.c_str());
			node.append_attribute("hasItem").set_value(npc->hasItem);
		}
	}

	// saving puzzles 
	for (auto p : gs->puzzles) {
		saveLevel.child("puzzles").append_child(p.first.c_str());
		saveLevel.child("puzzles").child(p.first.c_str()).append_attribute("done").set_value(p.second->Passed());
		saveLevel.child("puzzles").child(p.first.c_str()).append_attribute("passRequirements").set_value(p.second->passedReqiurements);
	}

	// saving special npcs
	Butler* butler = ((GameScreen*)game->GetScreen())->butler;
	saveLevel.append_child("Butler").append_attribute("display").set_value(butler->IsDisplay());
	saveLevel.child("Butler").append_attribute("phase").set_value((int)butler->currentPhase);


	// saving player and inventory
	Player* player = game->GetPlayer();
	saveLevel.child("Player").append_attribute("posX").set_value(player->getPos().x);
	saveLevel.child("Player").append_attribute("posY").set_value(player->getPos().y);

	for (int i = 0; i < gs->GetInventory()->GetSize(); i++) {
		pugi::xml_node node = saveLevel.child("Player").child("inventory").append_child("item");
		Item* item = gs->GetInventory()->GetInventoryBox(i)->GetItem();
		if (item != nullptr) {
			node.append_attribute("name").set_value(item->name.c_str());
			node.append_attribute("multipleUse").set_value(item->multipleUse);
		}
	}

	// save infoPhone
	Phone* phone = Phone::GetInstance();
	int noteSize = (int)phone->app->notes.size();
	int chatSize = (int)phone->app->chats.size();

	saveLevel.child("Phone").append_attribute("hasPhone").set_value(gs->phoneIcon->IsDisplay());
	saveLevel.child("Phone").append_attribute("firstClose").set_value(phone->firstClose);
	saveLevel.child("Phone").append_attribute("note23Track").set_value(phone->note23Done);
	saveLevel.child("Phone").append_child("npcTalked");
	for (int i = 0; i < phone->npcTalked.size(); i++) {
		saveLevel.child("Phone").child("npcTalked").append_child(phone->npcTalked[i].c_str());
	}

	for (int i = 0; i < noteSize; i++) {
		pugi::xml_node node = saveLevel.child("Phone").child("Notes").append_child("n");
		node.append_attribute("name").set_value(phone->app->notes[i]->name.c_str());
	}

	for (auto c:phone->chats) {
		pugi::xml_node node = saveLevel.child("Phone").child("Chats").append_child("c");
		node.append_attribute("name").set_value(c.second.name.c_str());
		node.append_attribute("msgNo").set_value(c.second.currentMsgIndex);
	}

	save.save_file(filename.c_str());
	SaveGameOptions();
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

	pugi::xml_node options = gameOption.append_child("Options");

	//options.append_child("Settings");
	//options.child("Settings").append_child("Master").append_attribute("mute").set_value(soundManager->getMute(MASTER));
	//options.child("Settings").child("Master").append_attribute("volume").set_value(soundManager->getVolume(MASTER));

	//options.child("Settings").append_child("BGM").append_attribute("mute").set_value(soundManager->getMute(BGM));
	//options.child("Settings").child("BGM").append_attribute("volume").set_value(soundManager->getVolume(BGM));

	//options.child("Settings").append_child("SFX").append_attribute("mute").set_value(soundManager->getMute(SFX));
	//options.child("Settings").child("SFX").append_attribute("volume").set_value(soundManager->getVolume(SFX));

	options.append_child("SaveSlots");
	std::vector<SaveLoadGameButton*> &buttons = SaveLoadWindow::GetInstance()->saveButtons;
	for (int i = 0; i < buttons.size(); i++) {
		pugi::xml_node s = options.child("SaveSlots").append_child("s");
		s.append_attribute("hasSaveFile").set_value(buttons[i]->hasSaved);
		s.append_attribute("details").set_value(buttons[i]->saveLevel.c_str());

	}

	gameOption.save_file("save/settings.xml");
}

void XMLManager::LoadGameOptions() {
	std::cout << "load game options\n";
	if (LoadFile("save/settings.xml")) {
		SoundManager* soundManager = SoundManager::GetInstance();

		pugi::xml_node settings = doc.child("Options").child("Settings");

		//soundManager->setVolume(MASTER, settings.child("Master").attribute("volume").as_float());
		//soundManager->setVolume(BGM, settings.child("BGM").attribute("volume").as_float());
		//soundManager->setVolume(SFX, settings.child("SFX").attribute("volume").as_float());

		//soundManager->setMute(MASTER, settings.child("Master").attribute("mute").as_bool());
		//soundManager->setMute(BGM, settings.child("BGM").attribute("mute").as_bool());
		//soundManager->setMute(SFX, settings.child("SFX").attribute("mute").as_bool());

		std::vector<SaveLoadGameButton*>& buttons = SaveLoadWindow::GetInstance()->saveButtons;
		pugi::xml_node save = doc.child("Options").child("SaveSlots").first_child();
		for (int i = 0; i < buttons.size(); i++, save = save.next_sibling()) {
			buttons[i]->hasSaved = save.attribute("hasSaveFile").as_bool();
			buttons[i]->SetSaveLevel(save.attribute("details").as_string());
		}
	}
}

XMLManager::~XMLManager() {

}

void XMLManager::LoadNotes(std::string filename, std::map<std::string, NoteInfo>& notes) {
	if (LoadFile(filename)) {
		pugi::xml_node allNotes = doc.child("notes");

		// generate all the rooms in that level
		for (pugi::xml_node_iterator note = allNotes.begin(); note != allNotes.end(); note++) {
			NoteInfo n;
			n.name = note->name();
			if (note->first_child())
				for (pugi::xml_node_iterator msg = note->begin(); msg != note->end(); msg++) {
					n.AddText(msg->child_value());
				}
			notes.insert(std::pair<std::string, NoteInfo>(n.name, n));
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

void XMLManager::LoadItems(std::vector<Item*> &items) {
	if (LoadFile("save/items.xml")) {
		for (pugi::xml_node_iterator item = doc.child("Items").begin(); item != doc.child("Items").end(); item++) {
			Item* i = new Item(item->name());

			if(item->child("separate"))
				for (pugi::xml_node_iterator s = item->child("separate").begin(); s != item->child("separate").end(); s++) {
					i->AddSeparatedItem(s->name());
				}

			int count = 0;
			if (item->child("combine")) {
				for (pugi::xml_node_iterator c = item->child("combine").begin(); c != item->child("combine").end(); c++) {
					i->AddItemsToCombine(c->attribute("itemToCombine").as_string(), c->attribute("afterCombined").as_string());
					count++;
				}
				//std::cout << " XMLManager::LoadItems " << item->name() << " " << count << std::endl;
			}
			

			if (item->child("multipleUse"))
				i->multipleUse = true;

			i->dialogue_name = item->attribute("dialogue").as_string();

			float x = item->attribute("sizeX").as_float();
			float y = item->attribute("sizeY").as_float();
			i->SetSize(x, y);

			if (item->attribute("IsizeX")) {
				float ix = item->attribute("IsizeX").as_float();
				float iy = item->attribute("IsizeY").as_float();
				i->SetISize(ix, iy);
			}

			for (pugi::xml_node_iterator d = item->child("dialogueAferUseWithObjs").begin(); d != item->child("dialogueAferUseWithObjs").end(); d++) {
				i->AddDialogueAfterUsedWithObj(d->name(), d->attribute("dialogue").as_string());
				if(d->first_child()){
					std::string triggerName = d->name();
					for (pugi::xml_node_iterator td = d->begin(); td != d->end(); td++) {
						i->AddChangeOtherDialogueAfterUsedWithObj(triggerName,
																  td->attribute("objName").as_string(), 
																  td->name(), 
																  td->attribute("dialogue").as_string());
					}
				}
			}

			i->SetInventoryTexture(item->attribute("i_texture").as_string());
			i->SetViewTexture(item->attribute("v_texture").as_string());
			items.push_back(i);
		}
	}
}

void XMLManager::LoadObjSpecialActions(std::string filename, Level* level) {
	if (LoadFile(filename)) {
		pugi::xml_node objs = doc.child("Objects");
		//std::cout << " XMLManager::LoadObjSpecialActions \n";
		for (pugi::xml_node_iterator room = objs.begin(); room != objs.end(); room++) {
			Room* r = level->rooms[room->name()];
			//std::cout << r->name << "==\n";
			for (pugi::xml_node_iterator obj = room->begin(); obj != room->end(); obj++) {
				InteractableObj* interactObj = ((InteractableObj*)(r->FindObject(obj->name())));
				//std::cout << interactObj->object_name << "==\n";
				for (pugi::xml_node_iterator triggerObj = obj->begin(); triggerObj != obj->end(); triggerObj++) {
					InteractableObj* o = ((InteractableObj*)(Game::GetInstance()->GetCurrentLevel()->FindObject(triggerObj->name())));
					interactObj->AddTriggerObj(o);
					o->triggered = false;
					//std::cout << o->object_name << std::endl;
				}
			}
		}
	}
}