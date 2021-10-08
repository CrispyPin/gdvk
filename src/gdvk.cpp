#include "gdvk.h"

using namespace godot;
using namespace std;

void GDVK::_register_methods() {
    register_method("key_down",  &GDVK::keyDown);
    register_method("key_up",    &GDVK::keyUp);
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
	if (!xdisplay) {
		Godot::print("Error opening X11 display");
	}
#endif
    generateKeymap();
}

void GDVK::keyPress(const String keyName) {
    keyDown(keyName);
    delay(1);
    keyUp(keyName);
}

void GDVK::keyDown(const String keyName) {
    setKeyState(keyName, true);
}

void GDVK::keyUp(const String keyName) {
    setKeyState(keyName, false);
}

void GDVK::setKeyState(const String keyName, bool pressState) {
    unsigned long keyCode = lookupKeyCode(keyName);
#ifdef __linux__

	if (!XTestFakeKeyEvent(xdisplay, keyCode, pressState, 0)) {
		Godot::print("Error sending keyboard event");
    }
	XFlush(xdisplay);
#else
	KEYBDINPUT keyInput;
	keyInput.wVk = keyCode;
	keyInput.wScan = 0; // something unicode
	keyInput.time = 0;
	keyInput.dwExtraInfo = 0;
	keyInput.dwFlags = 0; // keydown event if 0
    if (!pressState) {
        keyInput.dwFlags = KEYEVENTF_KEYUP;
    }

	INPUT inputEvent;
	inputEvent.type = INPUT_KEYBOARD;
	inputEvent.ki = keyInput;

	SendInput(1, &inputEvent, sizeof(inputEvent));

#endif
}

unsigned long GDVK::lookupKeyCode(const String keyName) {
    const char* key_name = keyName.utf8().get_data();
    if (keymap.find(key_name) != keymap.end()) {
        return keymap[key_name];
    }
    //Godot::print(key_name);
    //Godot::print("Warning: keyName not in keymap, guessing (platform dependent)");
    return stringToKeyCode(key_name);
}

unsigned long GDVK::stringToKeyCode(const char* key_name) {//does not work with longer key names
#ifdef __linux__
    return XKeysymToKeycode(xdisplay, XStringToKeysym(key_name));
#else
    return VkKeyScanExA(key_name[0], GetKeyboardLayout(0));
#endif
}

void GDVK::delay(unsigned int ms) {
#ifdef __linux__
    usleep(1000 * ms);
#else
    Sleep(ms);
#endif
}

void GDVK::generateKeymap() {
    Godot::print("Generating key map");
    // contains keycodes for special keys that have different names on windows and linux.
    // uses godot naming
#ifdef __linux__
/*
    Same name in X11:
    A - Z
    1 - 9
    F1 - F12
    KP_1 - KP_9
*/
    keymap["ESCAPE"]        = stringToKeyCode("Escape");
    keymap["TAB"]           = stringToKeyCode("Tab");
    keymap["BACKSPACE"]     = stringToKeyCode("BackSpace");
    keymap["ENTER"]         = stringToKeyCode("Return");
    keymap["KP_ENTER"]      = stringToKeyCode("KP_Enter");
    keymap["INSERT"]        = stringToKeyCode("Insert");
    keymap["DELETE"]        = stringToKeyCode("Delete");
    keymap["PRINT"]         = stringToKeyCode("Print");
    keymap["HOME"]          = stringToKeyCode("Home");
    keymap["END"]           = stringToKeyCode("End");
    keymap["LEFT"]          = stringToKeyCode("Left");
    keymap["UP"]            = stringToKeyCode("Up");
    keymap["RIGHT"]         = stringToKeyCode("Right");
    keymap["DOWN"]          = stringToKeyCode("Down");
    keymap["PAGEUP"]        = stringToKeyCode("Page_Up");
    keymap["PAGEDOWN"]      = stringToKeyCode("Page_Down");
    keymap["SHIFT"]         = stringToKeyCode("Shift_L");
    keymap["CONTROL"]       = stringToKeyCode("Control_L");
    keymap["ALT"]           = stringToKeyCode("Alt_L");
    keymap["CAPSLOCK"]      = stringToKeyCode("Caps_Lock");
    keymap["NUMLOCK"]       = stringToKeyCode("Num_Lock");
    keymap["SCROLLLOCK"]    = stringToKeyCode("Scroll_Lock");
    keymap["KP_MULTIPLY"]   = stringToKeyCode("KP_Multiply");
    keymap["KP_DIVIDE"]     = stringToKeyCode("KP_Divide");
    keymap["KP_SUBTRACT"]   = stringToKeyCode("KP_Subtract");
    keymap["KP_ADD"]        = stringToKeyCode("KP_Add");
    keymap["KP_PERIOD"]     = stringToKeyCode("KP_Separator");
    keymap["SUPER"]         = stringToKeyCode("Super_L");
    keymap["SUPER_L"]       = stringToKeyCode("Super_L");
    keymap["SUPER_R"]       = stringToKeyCode("Super_R");
    keymap["MENU"]          = stringToKeyCode("Menu");

    keymap["SPACE"]         = stringToKeyCode("space");
#else //windows
    keymap["ESCAPE"]        = VK_ESCAPE;
    keymap["TAB"]           = VK_TAB;
    keymap["BACKSPACE"]     = VK_BACK;
    keymap["ENTER"]         = VK_RETURN;
    keymap["KP_ENTER"]      = 0;
    keymap["INSERT"]        = VK_INSERT;
    keymap["DELETE"]        = VK_DELETE;
    keymap["PRINT"]         = VK_SNAPSHOT;
    keymap["HOME"]          = VK_HOME;
    keymap["END"]           = VK_END;
    keymap["LEFT"]          = VK_LEFT;
    keymap["UP"]            = VK_UP;
    keymap["RIGHT"]         = VK_RIGHT;
    keymap["DOWN"]          = VK_DOWN;
    keymap["PAGEUP"]        = VK_PRIOR;
    keymap["PAGEDOWN"]      = VK_NEXT;
    keymap["SHIFT"]         = VK_LSHIFT;
    keymap["CONTROL"]       = VK_CONTROL;
    keymap["ALT"]           = VK_MENU;
    keymap["CAPSLOCK"]      = VK_CAPITAL;
    keymap["NUMLOCK"]       = VK_NUMLOCK;
    keymap["SCROLLLOCK"]    = VK_SCROLL;
    keymap["KP_MULTIPLY"]   = VK_MULTIPLY;
    keymap["KP_DIVIDE"]     = VK_DIVIDE;
    keymap["KP_SUBTRACT"]   = VK_SUBTRACT;
    keymap["KP_ADD"]        = VK_ADD;
    keymap["KP_PERIOD"]     = VK_DECIMAL;
    keymap["SUPER"]         = VK_LWIN;
    keymap["SUPER_L"]       = VK_LWIN;
    keymap["SUPER_R"]       = VK_RWIN;
    keymap["MENU"]          = VK_RMENU;
    
    keymap["SPACE"] = VK_SPACE;

    keymap["KP_0"]    = VK_NUMPAD0;
    keymap["KP_1"]    = VK_NUMPAD1;
    keymap["KP_2"]    = VK_NUMPAD2;
    keymap["KP_3"]    = VK_NUMPAD3;
    keymap["KP_4"]    = VK_NUMPAD4;
    keymap["KP_5"]    = VK_NUMPAD5;
    keymap["KP_6"]    = VK_NUMPAD6;
    keymap["KP_7"]    = VK_NUMPAD7;
    keymap["KP_8"]    = VK_NUMPAD8;
    keymap["KP_9"]    = VK_NUMPAD9;

    keymap["F1"]    = VK_F1;
    keymap["F2"]    = VK_F2;
    keymap["F3"]    = VK_F3;
    keymap["F4"]    = VK_F4;
    keymap["F5"]    = VK_F5;
    keymap["F6"]    = VK_F6;
    keymap["F7"]    = VK_F7;
    keymap["F8"]    = VK_F8;
    keymap["F9"]    = VK_F9;
    keymap["F10"]   = VK_F10;
    keymap["F11"]   = VK_F11;
    keymap["F12"]   = VK_F12;
    keymap["F13"]   = VK_F13;
    keymap["F14"]   = VK_F14;
    keymap["F15"]   = VK_F15;
    keymap["F16"]   = VK_F16;
#endif
}
