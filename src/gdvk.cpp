#include "gdvk.h"

using namespace godot;

void GDVK::_register_methods() {
    //register_method("_process", &GDVK::_process);
    register_method("key_down",  &GDVK::keyDown);
    register_method("key_up",    &GDVK::keyDown);
    register_method("key_press", &GDVK::keyPress);
}

GDVK::GDVK() {
}

GDVK::~GDVK() {
    // add your cleanup here
}

void GDVK::_init() {
    // initialize any variables here
    //key_sender = std::make_unique<KeySenderLinux>();
    Godot::print("Opening X11 display");
    xdisplay = XOpenDisplay(NULL);
	if (!xdisplay)
	{//should raise proper error instead
		Godot::print("Error opening X11 display");
	}
}

void GDVK::keyDown(const String keyName) {
    char code = stringToKeyCode(keyName);
    
	if (!XTestFakeKeyEvent(xdisplay, code, true, 0)) {
		Godot::print("Error sending keyboard event");
    }
	XFlush(xdisplay);
}

void GDVK::keyUp(const String keyName) {
    char code = stringToKeyCode(keyName);

	if (!XTestFakeKeyEvent(xdisplay, code, false, 0)) {
		Godot::print("Error sending keyboard event");
    }
	XFlush(xdisplay);
}

void GDVK::keyPress(const String keyName) {
    keyDown(keyName);
    usleep(1000);
    keyUp(keyName);
}

#ifdef __linux__
char GDVK::stringToKeyCode(const String keyName) {
    return XKeysymToKeycode(xdisplay, XStringToKeysym( keyName.utf8().get_data() ));
}
#endif