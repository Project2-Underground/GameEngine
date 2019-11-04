#pragma once
#include "ImageObject.h"
#include "UIObject.h"

class CursorUI : public UIObject {
	private:
		ImageObject* normalCS;
		//ImageObject* chatCS;
		//ImageObject pickCS;
		//ImageObject moveCS;
		//ImageObject viewCS;
		//ImageObject saveCS;

	public:
		CursorUI();
		~CursorUI();
		void updateCursor();
		void setCursor();
};