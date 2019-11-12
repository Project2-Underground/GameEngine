#include "Camera.h"
#include "Game.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

Camera* Camera::_instance = nullptr;
Camera* Camera::GetInstance() {
	if (_instance == nullptr)
		_instance = new Camera();
	return _instance;
}

void Camera::SetTarget(DrawableObject* t) {
	target = t;
	offset_y = 0 - target->getPos().y;
}

void Camera::SetLimit(Collider* c) {
	limit = c;
}

Camera::Camera() {
	currrent_cam_pos = glm::vec3(0.0, 0.0, 0.0);
	target = nullptr;
	limit = nullptr;
}

Camera::~Camera() {
	if (_instance->limit != nullptr)
		delete _instance->limit;
	delete _instance;
}

glm::mat4 Camera::GetViewMatrix() {
	if (target != nullptr) {
		glm::vec3 next_pos = glm::vec3(-target->getPos().x, target->getPos().y + offset_y, 1.0);
		glm::mat4 view = glm::mat4(1.0);

		// check if out of game view
		if (limit != nullptr) {
			if (((next_pos.x - Game::GetInstance()->winWidth / 2) < limit->getMinBound().x) ||
				((next_pos.x + Game::GetInstance()->winWidth / 2) > limit->getMaxBound().x))
				next_pos.x = currrent_cam_pos.x;
			if (((next_pos.y - Game::GetInstance()->winWidth / 2) < limit->getMinBound().y) ||
				((next_pos.y + Game::GetInstance()->winWidth / 2) > limit->getMaxBound().y))
				next_pos.y = currrent_cam_pos.y;
		}

		currrent_cam_pos = next_pos;
		return glm::translate(view, next_pos);
	}
	return glm::mat4(1.0);
}