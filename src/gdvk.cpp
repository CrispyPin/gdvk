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
#ifdef __linux__
    XCloseDisplay(xdisplay);
#endif
}

void GDVK::_init() {
#ifdef __linux__
    Godot::print("Opening X11 display");
    xdisplay = XOpenDisplay(NULL);
	if (!xdisplay)
	{//should raise proper error instead
		Godot::print("Error opening X11 display");
	}
#endif
}

void GDVK::keyPress(const String keyName) {
    keyDown(keyName);
#ifdef __linux__
    usleep(1000);
#else
    Sleep(1);
#endif
    keyUp(keyName);
}

void GDVK::keyDown(const String keyName) {
    setKeyState(keyName, true);
}

void GDVK::keyUp(const String keyName) {
    setKeyState(keyName, false);
}

void GDVK::setKeyState(const String keyName, bool state) {
#ifdef __linux__
    char code = stringToKeyCode(keyName);

	if (!XTestFakeKeyEvent(xdisplay, code, state, 0)) {
		Godot::print("Error sending keyboard event");
    }
	XFlush(xdisplay);
#else
    DWORD code = stringToKeyCode(keyName);

	KEYBDINPUT keyInput;
	keyInput.wVk = WORD(code);
	keyInput.wScan = 0; // something unicode
	keyInput.time = 0;
	keyInput.dwExtraInfo = 0;
	keyInput.dwFlags = 0; // keydown event if 0
    if (state) {
        keyInput.dwFlags = KEYEVENTF_KEYUP;
    }

	INPUT inputEvent;
	inputEvent.type = INPUT_KEYBOARD;
	inputEvent.ki = keyInput;

	SendInput(1, &inputEvent, sizeof(inputEvent));

#endif
}


#ifdef __linux__
char GDVK::stringToKeyCode(const String keyName) {
    return XKeysymToKeycode(xdisplay, XStringToKeysym( keyName.utf8().get_data() ));
}
#else
DWORD GDVK::stringToKeyCode(const String keyName) {
    char key = keyName.utf8().get_data()[0];
    return (DWORD) VkKeyScanExA(key, GetKeyboardLayout(0));
}
#endif