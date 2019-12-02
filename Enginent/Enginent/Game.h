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

enum objectType {
	IMAGE_OBJ = 0,
	INTERACT_OBJ,
	ITEM,
	PORTAL,
	NPC,
};

using namespace std;
class Game
{
	static Game* instance;
	vector<DrawableObject*> objects;
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

};
