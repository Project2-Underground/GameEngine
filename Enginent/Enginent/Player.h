#include "ImageObject.h"
#include "Collider.h"
#include "TextObject.h"
#include "SDL.h"

#define WALK_SPEED 3

class Player : public ImageObject
{
	public:
		bool walk;
		Player();
		void Move();
		void Walk(int x, int y);
		void setTarget(int x, int y);
		Collider* col;
		void SetCollder(Collider* n_col);
		TextObject* setDialogue(string dialogue);
	private:
		glm::vec3 target;
		SDL_Color dialogueColor;
		TextObject * dialogueText;
};

