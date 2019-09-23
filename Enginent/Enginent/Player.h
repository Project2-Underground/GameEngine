#pragma once
// OpenGL headers
#include <GL/glew.h>
/*#include <GL/glu.h>
#include <GL/gl.h>*/

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <string>
#include "GLRenderer.h"
#include "Game.h"
#include "GLRenderer.h"
#include "SquareMeshVbo.h"
#include "TriangleMeshVbo.h"
#include "GameObject.h"
#include "CombineObject.h"
#include "ImageObject.h"

class Player {
	glm::vec3 target;
public:
	Player();
	void Move();
};