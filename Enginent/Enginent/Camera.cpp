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
	offset_y = currrent_cam_pos.y - target->getPos().y;
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

void Camera::SetPosition(glm::vec3 pos) {
	next_pos = pos;
}

glm::mat4 Camera::GetViewMatrix() {
	glm::mat4 view = glm::mat4(1.0);
	if (target != nullptr) {
		glm::vec3 next_pos = glm::vec3(-target->getPos().x, -target->getPos().y + -offset_y, 1.0);
		glm::mat4 view = glm::mat4(1.0);

		// check if out of game view
		if (limit != nullptr) {
			// right bound
			if ((next_pos.x - Game::GetInstance()->winWidth * 0.5) < limit->getMinBound().x) {
				next_pos.x = limit->getMinBound().x + Game::GetInstance()->winWidth * 0.5;
			}
			// left bound
			else if ((next_pos.x + Game::GetInstance()->winWidth * 0.5) > limit->getMaxBound().x) {
				next_pos.x = limit->getMaxBound().x - Game::GetInstance()->winWidth * 0.5;
			}
			// upper bound
			else if ((next_pos.y - Game::GetInstance()->winHeight * 0.5) < limit->getMinBound().y) {
				next_pos.y = limit->getMinBound().y - Game::GetInstance()->winHeight * 0.5;
			}
			// lower bound
			else if ((next_pos.y + Game::GetInstance()->winHeight * 0.5) > limit->getMaxBound().y) {
				next_pos.y = limit->getMaxBound().y + Game::GetInstance()->winHeight * 0.5;
			}
		}
		return glm::translate(view, next_pos);
	}
	currrent_cam_pos = next_pos;
	return glm::mat4(1.0);
}

glm::vec3 Camera::GetPosition() {
	return currrent_cam_pos;
}