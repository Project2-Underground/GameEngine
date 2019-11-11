#pragma once



class Camera {
protected:
	glm::mat4 my_matrix;
public:
	static Camera* _instance;
	static Camera* GetInstance();
	Camera();
	~Camera();
	void translate();
	void rotate();
};