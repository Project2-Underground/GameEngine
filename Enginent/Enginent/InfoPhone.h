#pragma once

#include "InteractObj.h"

class InfoPhone : public InteractableObj {
	std::string file;
public:
	void LoadNote();
	void LoadChat();
	void Action();
	void Chat();
	void Note();
};