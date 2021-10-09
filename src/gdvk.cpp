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
#ifdef _DEBUG
    //Godot::print(key_name);
    Godot::print("Warning: keyName: '" + keyName + "' not in keymap, guessing (platform dependent)");
#endif
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

#ifdef __linux__
unsigned long GDVK::keysymToKeyCode(unsigned long keysym) {
    return XKeysymToKeycode(xdisplay, keysym);
}
#endif

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
    keymap["ESCAPE"]        = keysymToKeyCode(XK_Escape);
    keymap["TAB"]           = keysymToKeyCode(XK_Tab);
    keymap["BACKSPACE"]     = keysymToKeyCode(XK_BackSpace);
    keymap["ENTER"]         = keysymToKeyCode(XK_Return);
    keymap["KP_ENTER"]      = keysymToKeyCode(XK_KP_Enter);
    keymap["INSERT"]        = keysymToKeyCode(XK_Insert);
    keymap["DELETE"]        = keysymToKeyCode(XK_Delete);
    keymap["PRINT"]         = keysymToKeyCode(XK_Print);
    keymap["HOME"]          = keysymToKeyCode(XK_Home);
    keymap["END"]           = keysymToKeyCode(XK_End);
    keymap["LEFT"]          = keysymToKeyCode(XK_Left);
    keymap["UP"]            = keysymToKeyCode(XK_Up);
    keymap["RIGHT"]         = keysymToKeyCode(XK_Right);
    keymap["DOWN"]          = keysymToKeyCode(XK_Down);
    keymap["PAGEUP"]        = keysymToKeyCode(XK_Page_Up);
    keymap["PAGEDOWN"]      = keysymToKeyCode(XK_Page_Down);
    keymap["SHIFT"]         = keysymToKeyCode(XK_Shift_L);
    keymap["CONTROL"]       = keysymToKeyCode(XK_Control_L);
    keymap["ALT"]           = keysymToKeyCode(XK_Alt_L);
    keymap["CAPSLOCK"]      = keysymToKeyCode(XK_Caps_Lock);
    keymap["NUMLOCK"]       = keysymToKeyCode(XK_Num_Lock);
    keymap["SCROLLLOCK"]    = keysymToKeyCode(XK_Scroll_Lock);
    keymap["KP_MULTIPLY"]   = keysymToKeyCode(XK_KP_Multiply);
    keymap["KP_DIVIDE"]     = keysymToKeyCode(XK_KP_Divide);
    keymap["KP_SUBTRACT"]   = keysymToKeyCode(XK_KP_Subtract);
    keymap["KP_ADD"]        = keysymToKeyCode(XK_KP_Add);
    keymap["KP_PERIOD"]     = keysymToKeyCode(XK_KP_Separator);
    keymap["SUPER"]         = keysymToKeyCode(XK_Super_L);
    keymap["SUPER_L"]       = keysymToKeyCode(XK_Super_L);
    keymap["SUPER_R"]       = keysymToKeyCode(XK_Super_R);
    keymap["MENU"]          = keysymToKeyCode(XK_Menu);

    keymap["BACK"]          = keysymToKeyCode(XF86XK_Back);// browser back
    keymap["FORWARD"]       = keysymToKeyCode(XF86XK_Forward);// browser forward
    keymap["VOLUMEDOWN"]    = keysymToKeyCode(XF86XK_AudioLowerVolume);
    keymap["VOLUMEUP"]      = keysymToKeyCode(XF86XK_AudioRaiseVolume);
    keymap["MEDIAPLAY"]     = keysymToKeyCode(XF86XK_AudioPlay);
    keymap["MEDIASTOP"]     = keysymToKeyCode(XF86XK_AudioStop);
    keymap["MEDIAPREVIOUS"] = keysymToKeyCode(XF86XK_AudioPrev);
    keymap["MEDIANEXT"]     = keysymToKeyCode(XF86XK_AudioNext);

    keymap["SPACE"]         = keysymToKeyCode(XK_space);
    keymap["EXCLAM"]        = keysymToKeyCode(XK_exclam);       // !
    keymap["QUOTEDBL"]      = keysymToKeyCode(XK_quotedbl);     // "
    keymap["NUMBERSIGN"]    = keysymToKeyCode(XK_numbersign);   // #
    keymap["DOLLAR"]        = keysymToKeyCode(XK_dollar);       // $
    keymap["PERCENT"]       = keysymToKeyCode(XK_percent);      // %
    keymap["AMPERSAND"]     = keysymToKeyCode(XK_ampersand);    // &
    keymap["APOSTROPHE"]    = keysymToKeyCode(XK_apostrophe);   // '
    keymap["PARENLEFT"]     = keysymToKeyCode(XK_parenleft);    // (
    keymap["PARENRIGHT"]    = keysymToKeyCode(XK_parenright);   // )
    keymap["ASTERISK"]      = keysymToKeyCode(XK_asterisk);     // *
    keymap["PLUS"]          = keysymToKeyCode(XK_plus);         // +
    keymap["COMMA"]         = keysymToKeyCode(XK_comma);        // ,
    keymap["MINUS"]         = keysymToKeyCode(XK_minus);        // -
    keymap["PERIOD"]        = keysymToKeyCode(XK_period);       // .
    keymap["SLASH"]         = keysymToKeyCode(XK_slash);        // /

    keymap["COLON"]         = keysymToKeyCode(XK_colon);        // :
    keymap["SEMICOLON"]     = keysymToKeyCode(XK_semicolon);    // ;
    keymap["LESS"]          = keysymToKeyCode(XK_less);         // <
    keymap["EQUAL"]         = keysymToKeyCode(XK_equal);        // =
    keymap["GREATER"]       = keysymToKeyCode(XK_greater);      // >
    keymap["QUESTION"]      = keysymToKeyCode(XK_question);     // ?
    keymap["AT"]            = keysymToKeyCode(XK_at);           // @

    keymap["BRACKETLEFT"]   = keysymToKeyCode(XK_bracketleft);  // [
    keymap["BACKSLASH"]     = keysymToKeyCode(XK_backslash);    // \  .
    keymap["BRACKETRIGHT"]  = keysymToKeyCode(XK_bracketright); // ]
    keymap["ASCIICIRCUM"]   = keysymToKeyCode(XK_asciicircum);  // ^
    // localisation
    keymap["DIAERSIS"]      = keysymToKeyCode(XK_dead_diaeresis);// ¨
    keymap["ARING"]         = keysymToKeyCode(XK_Aring);        // Å
    keymap["ADIAERSIS"]     = keysymToKeyCode(XK_Adiaeresis);   // Ä
    keymap["ODIAERSIS"]     = keysymToKeyCode(XK_Odiaeresis);   // Ö
    keymap["UDIAERSIS"]     = keysymToKeyCode(XK_Udiaeresis);   // Ü


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


    keymap["BACK"]          = VK_BROWSER_BACK;
    keymap["FORWARD"]       = VK_BROWSER_FORWARD;
    keymap["VOLUMEDOWN"]    = VK_VOLUME_DOWN;
    keymap["VOLUMEUP"]      = VK_VOLUME_UP;
    keymap["MEDIAPLAY"]     = VK_MEDIA_PLAY_PAUSE;
    keymap["MEDIASTOP"]     = VK_MEDIA_STOP;
    keymap["MEDIAPREVIOUS"] = VK_MEDIA_NEXT_TRACK;
    keymap["MEDIANEXT"]     = VK_MEDIA_PREV_TRACK;

    keymap["SPACE"]         = VK_SPACE;
    keymap["EXCLAM"]        = 0;
    keymap["QUOTEDBL"]      = VK_OEM_7;// what
    keymap["NUMBERSIGN"]    = 0;
    keymap["DOLLAR"]        = 0;
    keymap["PERCENT"]       = 0;
    keymap["AMPERSAND"]     = 0;
    keymap["APOSTROPHE"]    = 0;
    keymap["PARENLEFT"]     = 0;// why
    keymap["PARENRIGHT"]    = 0;
    keymap["ASTERISK"]      = 0; 
    keymap["PLUS"]          = VK_OEM_PLUS;
    keymap["COMMA"]         = VK_OEM_COMMA;
    keymap["MINUS"]         = VK_OEM_MINUS;
    keymap["PERIOD"]        = VK_OEM_PERIOD;
    keymap["SLASH"]         = 0;

    keymap["COLON"]         = 0;
    keymap["SEMICOLON"]     = 0;
    keymap["LESS"]          = 0;
    keymap["EQUAL"]         = 0;
    keymap["GREATER"]       = 0;
    keymap["QUESTION"]      = 0;
    keymap["AT"]            = 0;

    keymap["BRACKETLEFT"]   = 0;
    keymap["BACKSLASH"]     = 0;
    keymap["BRACKETRIGHT"]  = 0;
    keymap["ASCIICIRCUM"]   = 0;
    // localisation
    keymap["DIAERSIS"]      = 0;
    keymap["ARING"]         = 0;
    keymap["ADIAERSIS"]     = 0;
    keymap["ODIAERSIS"]     = 0;
    

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
