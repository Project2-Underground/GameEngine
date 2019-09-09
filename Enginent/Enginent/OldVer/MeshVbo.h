#pragma once

#include <GL\glew.h>
#include <string>

using namespace std;

class MeshVbo {
protected:
	GLuint posVboId = -1;
	GLuint texVboId = -1;
	GLuint posAttribId = -1;
	GLuint texAttribId = -1;

public:

	MeshVbo();
	virtual void LoadData() = 0;
	virtual string GetMeshName() = 0;
	GLuint GetPosId();
	GLuint GetTexId();
	virtual void Render() = 0;
	void SetAttribId(GLuint posId, GLuint texId);

};