#include "ImageObject.h"
#include "Collider.h"
#include "TextObject.h"
#include "SDL.h"

class Player : public ImageObject
{
public:
	Player();
	void Move();
	void setTarget(int x, int y);
	Collider* col;
	void SetCollder(Collider* n_col);
	TextObject* setDialogue(string dialogue);
private:
	glm::vec3 target;
	SDL_Color dialogueColor;
	TextObject * dialogueText;
};

