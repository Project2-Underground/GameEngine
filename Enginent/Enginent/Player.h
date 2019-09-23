#include "ImageObject.h"

class Player : public ImageObject
{
public:
	Player();
	void Move();
	void setTarget(int x, int y);

private:
	glm::vec3 target;
};

