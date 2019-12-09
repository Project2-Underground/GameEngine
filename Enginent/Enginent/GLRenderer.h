#pragma once

#include <iostream>
#include <GL\glew.h>
#include "Shader.h"
#include <map>
#include <vector>
#include <string>
#include "MeshVbo.h"
#include "glm.hpp"
#include "DrawableObject.h"
#include "SDL_surface.h"
#include "SDL_image.h"
#include "UIObject.h"

using namespace std;

class GLRenderer
{
protected:
	int winWidth;
	int winHeight;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	GLuint mMatrixId = -1;
	GLuint pMatrixId = -1;
	GLuint colorUniformId = -1;
	GLuint modeUniformId = -1;

	GLuint gProgramId;
	int gPos2DLocation;
	int gTex2DLocation;
	void PrintProgramLog(GLuint program);
	bool Initialize(string vertexShaderFile, string fragmentShaderFile);
	Shader *vertexShader;
	Shader *fragmentShader;
	map <string, MeshVbo*> shapes;
	void SetMeshAttribId(MeshVbo * shape);

public:
	void Render(vector <DrawableObject*> & objList);
	void Render(vector <UIObject*> & objList);
	void Render(UIObject* obj);
	void Render(DrawableObject* obj);
	GLRenderer(int w, int h);
	bool InitGL(string vertexShaderFile, string fragmentShaderFile);

	void AddMesh(string name, MeshVbo* shape);
	MeshVbo * GetMesh(string name);

	~GLRenderer();

	void SetOrthoProjection(float left, float right, float bottom, float top);
	void SetViewPort(int x, int y, int w, int h);
	void SetClearColor(float r, float g, float b);

	GLuint GetModelMatrixAttrId();
	GLuint GetProjectionMatrixAttrId();
	GLuint GetColorUniformId();
	GLuint GetModeUniformId();

	GLuint LoadTexture(string path);

};
