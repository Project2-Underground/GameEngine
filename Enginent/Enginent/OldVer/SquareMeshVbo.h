#pragma once

#include "MeshVbo.h"

class SquareMeshVbo : public MeshVbo {
protected:

public:
	static string const MESH_NAME;
	SquareMeshVbo();
	virtual string GetMeshName();
	virtual void Render();
	virtual void LoadData();
};