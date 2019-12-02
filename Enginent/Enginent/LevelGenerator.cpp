#include "LevelGenerator.h"
#include "Item.h"
#include "Door.h"

#include <iostream>

//void RoomGenerator::GenerateRoom(std::string filename) {
//	/* generate room base on the xml doc */
//	if (LoadFile(filename)) {
//		pugi::xml_node object = doc.child("room");	// get the first node of the root
//		for (pugi::xml_node_iterator child = object.begin(); child != object.end(); child++) {
//			std::cout << "in";
//			 //loop through headers
//			int type = child->attribute("type").as_int();
//			for (pugi::xml_node_iterator aChild = child->begin(); aChild != child->end(); aChild++) {
//				// get attributes
//				// createObject
//				std::string texture = aChild->attribute("texture").as_string();
//				int sizeX = aChild->attribute("sizeX").as_int();
//				int sizeY = aChild->attribute("sizeY").as_int();
//				glm::vec3 pos = glm::vec3(aChild->attribute("posX").as_float(),
//					aChild->attribute("posY").as_float(),
//					1.0f);
//				std::vector<std::string> dialogueVec;
//				if (aChild->child("dialogue")) {
//					for (pugi::xml_node_iterator dialogue = aChild->child("dialogue").begin(); dialogue != aChild->child("dialogue").end(); dialogue++) {
//						dialogueVec.push_back(dialogue->child_value());
//					}
//				}
//				//Game::GetInstance()->createObject((objectType)type, texture, sizeX, -sizeY, pos, dialogueVec);
//			}
//		}
//	}
//}

void RoomGenerator::GenerateRoom(std::string filename, std::string roomNo, std::vector<DrawableObject*> &objects){
	if (LoadFile(filename)) {
		pugi::xml_node room = doc.child("rooms").child(roomNo.c_str());
		//std::cout << "in GenerateRoom\n";
		GenerateBackground(room, objects);
		GenerateInteractObj(room, objects);
		GenerateDoor(room, objects);
		GenerateItem(room, objects);
		GenerateNPC(room, objects);
	}
}

void RoomGenerator::GenerateBackground(pugi::xml_node room, std::vector<DrawableObject*> &objects) {
	pugi::xml_node background = room.child("background");
	
	for (pugi::xml_node_iterator child = background.begin(); child != background.end(); child++) {
		//std::cout << "in GenerateBackground\n";
		ImageObject* bg = new ImageObject();
		CreateObject(bg, *child);
		objects.push_back(bg);
	}
}

void RoomGenerator::GenerateInteractObj(pugi::xml_node room, std::vector<DrawableObject*> &objects) {
	pugi::xml_node interactObj = room.child("interactObj");

	for (pugi::xml_node_iterator child = interactObj.begin(); child != interactObj.end(); child++) {
		//std::cout << "in GenerateInteractObj\n";
		InteractableObj* interactObj = new InteractableObj();
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

void RoomGenerator::GenerateDoor(pugi::xml_node room, std::vector<DrawableObject*> &objects) {
	pugi::xml_node doors = room.child("doors");

	for (pugi::xml_node_iterator child = doors.begin(); child != doors.end(); child++) {
		//std::cout << "in GenerateDoor\n";
		int next_room = child->attribute("next_room").as_int();
		int next_door = child->attribute("next_door").as_int();

		Door* door = new Door(next_room, next_door);
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
	}
}

void RoomGenerator::GenerateItem(pugi::xml_node room, std::vector<DrawableObject*>& objects) {
	pugi::xml_node items = room.child("item");

	for (pugi::xml_node_iterator child = items.begin(); child != items.end(); child++) {
		Item* item;
		//std::cout << "in GenerateItem\n";
		std::vector<Item*> resultItems;
		// if the item can be separate or combine
		if (child->first_child()) {
			for (pugi::xml_node_iterator cChild = child->begin(); cChild != child->end(); cChild++) {
				Item* tmp = new Item(cChild->name());
				tmp->SetTexture(cChild->attribute("texture").as_string());
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

void RoomGenerator::GenerateNPC(pugi::xml_node room, std::vector<DrawableObject*> &objects) {

}

void RoomGenerator::CreateObject(ImageObject* tmp, pugi::xml_node node) {
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