#include <algorithm>

#include "Level.h"
#include "XMLManager.h"
#include "Game.h"
#include "TextBox.h"

void Room::Render() {
	// render objects
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(objects, true);
	renderer->Render(npcs, false);
	renderer->Render(Game::GetInstance()->GetPlayer());
	renderer->Render(foreground, false);
}

Room::~Room() {
	delete playerWalkLimit;
	delete cameraLimit;
	for (auto obj : objects)
		delete obj;
}

void Room::Update() {
	for (auto obj : objects)
		obj->Update();
	for (auto npc : npcs) {
		npc->Update();
	}
}

void Room::SetPlayerWalkLimit(Collider* col) {
	delete playerWalkLimit;
	playerWalkLimit = col;
}

void Room::SetCameraLimit(Collider* col) {
	delete cameraLimit;
	cameraLimit = col;
}

void Room::RightClick(float x, float y) {
	Game* game = Game::GetInstance();
	TextBox* textbox = TextBox::GetInstance();
	((GameScreen*)game->GetScreen())->GetPlayer()->SetNextPosition((float)x, (float)y);
}

void Room::LeftClick(float x, float y) {
	Game* game = Game::GetInstance();
	Player* player = game->GetPlayer();

	for (int i = 0; i < objects.size(); i++)
		if (InteractableObj * ib = dynamic_cast<InteractableObj*>(objects[i]))
			if (ib->CheckCollider((float)x, (float)y)) 
				player->CheckTarget(ib);

	for (int i = 0; i < npcs.size(); i++) {
		InteractableObj* npc = dynamic_cast<InteractableObj*>(npcs[i]);
		if (npc->CheckCollider((float)x, (float)y)) {
			player->CheckTarget(npc);
		}
	}

	if (player->CheckCollider((float)x, (float)y)) {
		player->action();
	}
}

void Room::SortObjLayer() {
	// sort the main layers
	DrawableObject* key;
	int ptr;
	for (int i = 1; i < objects.size(); i++) {
		key = objects[i]; 
		ptr = i - 1;
		while (key->layer < objects[ptr]->layer && ptr >= 0) {
			objects[ptr + 1] = objects[ptr];
			ptr--;
		}
		objects[ptr + 1] = key;
	}

	// sort the layers within each main layers
	int iStart = 0;
	Layers currentLayer = BACKGROUND_LAYER;
	for (int i = 0; i < objects.size(); i++) {
		if ((Layers)objects[i]->layer != currentLayer || i == objects.size() - 1) {
			for (int j = iStart + 1; j <= i; j++) {
				key = objects[j];
				ptr = j - 1;
				while (key->subLayer < objects[ptr]->subLayer && ptr >= iStart) {
					objects[ptr + 1] = objects[ptr];
					ptr--;
				}
				objects[ptr + 1] = key;
			}
			currentLayer = (Layers)objects[i]->layer;
			iStart = i;
		}
	}
	// sort the layers within each main layers
	if (foreground.size() > 1) {
		for (int i = 0; i < foreground.size() - 1; i++) {
			bool swap = false;
			for (int j = 0; j < foreground.size() - 1 - i; j++) {
				if (foreground[j]->subLayer > foreground[j + 1]->subLayer) {
					DrawableObject* tmp = foreground[j];
					foreground[j] = foreground[j + 1];
					foreground[j + 1] = tmp;
					swap = true;
				}
			}
			if (!swap)
				break;
		}
	}

	//std::cout << "--------------------------------layers--------------------------------\n";
	//for (auto obj : foreground) {
	//	std::cout << "obj " << obj->object_name << " sublayer: " << obj->subLayer << std::endl;
	//}
}

DrawableObject* Room::FindObject(std::string name) {
	for (auto obj : objects)
		if (obj->object_name == name)
			return obj;
	for (auto npc : npcs)
		if (npc->object_name == name)
			return npc;
	for (auto door : doors)
		if (door.first == name)
			return door.second;
	return nullptr;
}

//level

Level::Level(std::string filename) {
	XMLManager* lg = XMLManager::GetInstance();
	lg->GenerateRoom(filename, rooms);
	lg->GetLevelNumber(filename, this);
	currentRoom = rooms[lg->GetFirstRoomName()];
}

void Level::Update() {
	currentRoom->Update();
}

void Level::Render() {
	currentRoom->Render();
}

void Level::RightClick(float x, float y) {
	currentRoom->RightClick(x, y);
}

void Level::LeftClick(float x, float y) {
	currentRoom->LeftClick(x, y);
}

void Level::ChangeRoom(std::string roomName, std::string door) {

	if (currentRoom->name == "Building4Puzzle6Floor1" && roomName == "MainFloor2") {
		((InteractableObj*)FindObject("MainFloor2_BlackAlley"))->SetCurrentDialogueName("Floor2_BackAlley_FinishedPuzzle");
	}

	currentRoom = rooms[roomName];

	SoundManager* sm = SoundManager::GetInstance();
	if (roomName != "MainFloor2" && roomName != "MainFloor3") {
		sm->StopBGM("talkingAmbient");
	}
	else {
		sm->playSound(BGM, "talkingAmbient");
	}

	if(!currentRoom->bgmName.empty() && sm->currentBGM != currentRoom->bgmName){
		sm->StopCurrentBGM();
		sm->playSound(BGM, currentRoom->bgmName);
	}

	Player* player = ((GameScreen*)Game::GetInstance()->GetScreen())->GetPlayer();

	float playerNextx = 0;
	if (!door.empty()) {
		if (currentRoom->doors[door]->hasNextX) {
			playerNextx = currentRoom->doors[door]->playerNextX;
		}
		else {
			playerNextx = currentRoom->doors[door]->getPos().x;
		}
	}

	glm::vec3 nextPlayerPosition = glm::vec3(playerNextx, currentRoom->y, 1);
	player->SetPosition(nextPlayerPosition);
	
	player->SetWalkLimit(currentRoom->GetPlayerWalkLimit());
	player->StopWalking();

	if (currentRoom->dialogue != "") {
		TextBox::GetInstance()->setText(currentRoom->dialogue);
		TextBox::GetInstance()->SetDisplay(true);
		currentRoom->dialogue = "";
	}

	Game::GetInstance()->GetCamera()->SetLimit(currentRoom->GetCameraLimit());

	if (roomName == "Building2") {
		player->TriggerRouteA();
	}

	if (roomName == "Building4Puzzle6Floor3") {
		InteractableObj* n = ((InteractableObj*)FindObject("Puzzle6_NPC1F2"));
		n->SetDisplay(false);
		n->col->enable = false;
		n = ((InteractableObj*)FindObject("Puzzle6_NPC2F2"));
		n->SetDisplay(false);
		n->col->enable = false;
	}

	if (roomName == "Building3") {
		InteractableObj* n = ((InteractableObj*)FindObject("NPC4"));
		n->SetDisplay(false);
		n->col->enable = false;

		n = ((InteractableObj*)FindObject("NPC5"));
		n->SetDisplay(false);
		n->col->enable = false;

		n = ((InteractableObj*)FindObject("NPC6"));
		n->SetDisplay(false);
		n->col->enable = false;

		n = ((InteractableObj*)FindObject("NPC7"));
		n->SetDisplay(false);
		n->col->enable = false;
	}

	if (roomName == "MainFloor2" && player->triggerRouteA)
		TextBox::GetInstance()->setText("Route_A");
}

std::vector<DrawableObject*>* Level::Getobjects() {
	return &currentRoom->objects;
}

Level::~Level() {
	for (std::map<std::string, Room*>::iterator itr = rooms.begin(); itr != rooms.end(); itr++) {
		delete itr->second;
	}
}

void Level::TriggerChangeDialogue(std::string objName, std::string dName) {
	InteractableObj* obj = dynamic_cast<InteractableObj*>(FindObject(objName));
	if (obj != nullptr) {
		// change dialogue of the obj here
	}
}

DrawableObject* Level::FindObject(std::string name) {
	std::map<std::string, Room*>::iterator itr;
	for (itr = rooms.begin(); itr != rooms.end(); itr++) {
		if (itr->second->FindObject(name) != nullptr)
			return itr->second->FindObject(name);
	}
	return nullptr;
}