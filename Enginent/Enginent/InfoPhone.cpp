#include "InfoPhone.h"

InfoPhone::InfoPhone() {
	// initialize all the icons and the phone
	noteIcon = new UIObject();
	chatIcon = new UIObject();
	exitButton = new UIObject();
	// set texture, position, size and others


	// set display to false
	display = false;
}

void InfoPhone::Open() {
	// set display
	display = true;
}

void InfoPhone::Chat() {

}

void InfoPhone::Note() {

}