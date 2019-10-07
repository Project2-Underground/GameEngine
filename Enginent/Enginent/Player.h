#include "ImageObject.h"
#include "Collider.h"

class Player : public ImageObject
{
public:
	Player();
	void Move();
	void setTarget(int x, int y);
	Collider* col;
	void SetCollder(Collider* n_col);
private:
	glm::vec3 target;
};

