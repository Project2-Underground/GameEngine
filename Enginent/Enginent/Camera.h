#pragma once

#include "glm.hpp"
#include "DrawableObject.h"
#include "Collider.h"

class Camera {
protected:
	DrawableObject* target;
	glm::vec3 currrent_cam_pos;
	glm::vec3 next_pos;

	Collider* limit;
	float offset_y;

	int cam_width;
	int cam_height;
public:
	static Camera* _instance;
	static Camera* GetInstance();

	void SetTarget(DrawableObject*);
	void SetLimit(Collider*);
	void SetPosition(glm::vec3);

	glm::vec3 GetPosition();

	Camera();
	~Camera();

	glm::mat4 GetViewMatrix();
};