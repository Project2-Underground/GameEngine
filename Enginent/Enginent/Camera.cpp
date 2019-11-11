#include "Camera.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

Camera* Camera::_instance = nullptr;
Camera* Camera::GetInstance() {
	if (_instance == nullptr)
		_instance = new Camera();
	return _instance;
}

Camera::Camera() {
}

Camera::~Camera() {

}

void Camera::translate() {

}

void Camera::rotate() {

}