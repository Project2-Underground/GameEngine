#pragma once

#include "MeshVbo.h"

class TriangleMeshVbo : public MeshVbo {
protected:

public:
	static string const MESH_NAME;
	TriangleMeshVbo();
	virtual string GetMeshName();
	virtual void Render();
	virtual void LoadData();
};