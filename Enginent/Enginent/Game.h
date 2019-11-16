#pragma once

#include <vector>
#include "Player.h"
#include "DrawableObject.h"
#include "GLRenderer.h"
#include "Collider.h"
#include "Cursor.h"

using namespace std;
class Game
{
	static Game* instance;
	int winWidth, winHeight;
	vector<DrawableObject*> objects;
	vector<UIObject*> UI;
	vector<Collider*> colliders;
	GLRenderer *renderer;
	Player * player;
	CursorUI* cursorGame;
	Game();
public:
	~Game();
	static Game* GetInstance();
	GLRenderer * GetRenderer();
	void rightClick(int x, int y);
	void leftClick(int x, int y);
	void HandleKey(char ch);
	void Init(int width, int height);
	void Update();
	void Render();
	void createObject(int type, std::string texture, int sizeX, int sizeY, glm::vec3 pos, IneractTypeList objType, vector<std::string>* dialogue);
	void AddObject(DrawableObject* obj);
	void AddUI(UIObject* obj);
	Player* getPlayer() { return player;}
	glm::vec3 findRealPos(int x, int y);
	int checkPointObject(glm::vec3 pos);

};
