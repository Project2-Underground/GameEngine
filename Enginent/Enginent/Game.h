#pragma once

#include <vector>
#include "Player.h"
#include "DrawableObject.h"
#include "GLRenderer.h"
#include "Collider.h"
#include "Camera.h"
#include "Cursor.h"
#include "Button.h"
#include "LevelGenerator.h"

using namespace std;
class Game
{
	static Game* instance;
	vector<DrawableObject*> objects;
	vector<Collider*> colliders;
	vector<UIObject*> UI;
	GLRenderer *renderer;
	Player * player;
	Camera* camera;
	Game();
	CursorUI* cursorGame;
public:
	int winWidth, winHeight;
	~Game();
	static Game* GetInstance();
	GLRenderer * GetRenderer();
	void rightClick(int x, int y);
	void leftClick(int x, int y);
	void HandleKey(char ch);
	void Init(int width, int height);
	void Update();
	void Render();
	
	void AddObject(DrawableObject* obj);
	void AddUI(UIObject* obj);
	void updateMouseState(int, int);
	Player* getPlayer() { return player;}
	glm::vec3 FindMousePosition(int x, int y);
	int checkPointObject(glm::vec3 pos);

	void CreateObject(std::string texture, int sizeX, int sizeY, glm::vec3 pos);																				// background
	void CreateObject(std::string name, std::string texture, int sizeX, int sizeY, glm::vec3 pos, std::vector<std::string> dialogue);							// interactable
	void CreateObject(std::string name, std::string texture, std::string i_texture, int type, int sizeX, int sizeY, glm::vec3 pos, std::vector<std::string> dialogue, std::vector<Item*> Items);	// items
	void CreateObject(std::string texture, int sizeX, int sizeY, glm::vec3 pos, int next_room, int door_no, std::vector<std::string> dialogue);																	// door
	void CreateObject(std::string texture, int sizeX, int sizeY, glm::vec3 pos, std::vector<std::string> dialogue);												// NPC
	void CreateObject(ImageObject* tmp, std::string texture, int sizeX, int sizeY, glm::vec3 pos);
};

enum objectType {
	IMAGE_OBJ = 0,
	INTERACT_OBJ,
	ITEM,
	PORTAL,
	NPC,
};
