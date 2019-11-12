#pragma once

#include <vector>
#include "Player.h"
#include "DrawableObject.h"
#include "GLRenderer.h"
#include "Collider.h"

using namespace std;
class Game
{
	static Game* instance;
	vector<DrawableObject*> objects;
	vector<Collider*> colliders;
	GLRenderer *renderer;
	Player * player;
	Game();
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
	void createObject(int type, std::string texture, int sizeX, int sizeY, glm::vec3 pos, vector<std::string>* dialogue);
	Player* getPlayer() { return player;}

};
