#include "xmlDialogue.h"

std::string xmlDialogueFile::GetDialogue(std::string name, int pos) {
	std::string str(GetPcData(doc.child(name.c_str()), pos));
	return str;
}