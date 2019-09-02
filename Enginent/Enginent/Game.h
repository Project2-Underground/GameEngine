#pragma once

#include <vector>
#include "DrawableObject.h"
#include "GLRenderer.h"

using namespace std;
class Game
{
	static Game* instance;
	int winWidth, winHeight;
	vector<DrawableObject*> objects;
	GLRenderer *renderer;
	Game();
public:
	static Game* GetInstance();
	GLRenderer * GetRenderer();
	void HandleMouse(int x, int y);
	void HandleKey(char ch);
	void Init(int width, int height);
	void Render();

};