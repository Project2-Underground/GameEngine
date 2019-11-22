#pragma once

#include "xmlFileManager.h"

class xmlDialogueFile : public xmlManager {
public:
	std::string GetDialogue(std::string name, int pos);
};