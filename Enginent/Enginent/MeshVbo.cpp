
#include "MeshVbo.h"


MeshVbo::MeshVbo()
{
}

GLuint MeshVbo::GetPosId()
{
	return this->posVboId;
}

GLuint MeshVbo::GetTexId()
{
	return this->texVboId;
}

void MeshVbo::SetAttribId(GLuint posId, GLuint texId)
{
	this->posAttribId = posId;
	this->texAttribId = texId;
}
