#include "xmlDialogue.h"

std::string xmlDialogueFile::GetDialogue(std::string name, int pos) {
	std::string str(GetPcData(doc.find_node(name), pos));
	return str;
}