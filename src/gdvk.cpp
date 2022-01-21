#include "gdvk.h"

using namespace godot;
using namespace std;

void GDVK::_register_methods() {
	register_method("key_down", &GDVK::keyDown);
	register_method("key_up",   &GDVK::keyUp);
	register_method("press", 	&GDVK::keyPress);
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
	Godot::print("Opening X11 display.");
	xdisplay = XOpenDisplay(NULL);
	if (!xdisplay) {
		Godot::print("Error opening X11 display");
	}
#endif
	generateKeymap();
	Godot::print("GDVK loaded.");
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
	KEYCODE keyCode = lookupKeycode(keyName);

#ifdef __linux__
	if (!XTestFakeKeyEvent(xdisplay, keyCode, pressState, 0)) {
		Godot::print("Error sending keyboard event for key '" + keyName + "'");
	}
	XFlush(xdisplay);

#else
	KEYBDINPUT keyInput;
	keyInput.wVk = keyCode;
	keyInput.dwFlags = !pressState & KEYEVENTF_KEYUP; // keydown event if 0
	keyInput.wScan = 0; // something unicode
	keyInput.dwExtraInfo = 0;
	keyInput.time = 0;
	//if (!pressState) {
	//	keyInput.dwFlags = KEYEVENTF_KEYUP;
	//}

	INPUT inputEvent;
	inputEvent.type = INPUT_KEYBOARD;
	inputEvent.ki = keyInput;

	SendInput(1, &inputEvent, sizeof(inputEvent));
#endif
}

KEYCODE GDVK::lookupKeycode(const String keyName) {
	const char* key_name = keyName.utf8().get_data();

	if (keymap.find(key_name) != keymap.end()) {
		return keymap[key_name];
	}
#ifdef _DEBUG
	Godot::print("Warning: keyName: '" + keyName + "' not in keymap, guessing (platform dependent)");
#endif
	return stringToKeycode(key_name);
}

KEYCODE GDVK::stringToKeycode(const char* key_name) {//does not work with longer key names on windows
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
KEYCODE GDVK::keysymToKeycode(unsigned long keysym) {
	return XKeysymToKeycode(xdisplay, keysym);
}
#endif

void GDVK::generateKeymap() {
#ifdef _DEBUG
	Godot::print("Generating key map");
#endif
	// contains keycodes for special keys that have different names on windows and linux.
	// uses godot naming
#ifdef __linux__
/*
	Same name in X11 (omitted here):
	A - Z
	1 - 9
	F1 - F12
	KP_1 - KP_9
*/
	keymap["ESCAPE"]        = keysymToKeycode(XK_Escape);
	keymap["TAB"]           = keysymToKeycode(XK_Tab);
	keymap["BACKSPACE"]     = keysymToKeycode(XK_BackSpace);
	keymap["ENTER"]         = keysymToKeycode(XK_Return);
	keymap["KP_ENTER"]      = keysymToKeycode(XK_KP_Enter);
	keymap["INSERT"]        = keysymToKeycode(XK_Insert);
	keymap["DELETE"]        = keysymToKeycode(XK_Delete);
	keymap["PRINT"]         = keysymToKeycode(XK_Print);
	keymap["HOME"]          = keysymToKeycode(XK_Home);
	keymap["END"]           = keysymToKeycode(XK_End);
	keymap["LEFT"]          = keysymToKeycode(XK_Left);
	keymap["UP"]            = keysymToKeycode(XK_Up);
	keymap["RIGHT"]         = keysymToKeycode(XK_Right);
	keymap["DOWN"]          = keysymToKeycode(XK_Down);
	keymap["PAGEUP"]        = keysymToKeycode(XK_Page_Up);
	keymap["PAGEDOWN"]      = keysymToKeycode(XK_Page_Down);
	keymap["SHIFT"]         = keysymToKeycode(XK_Shift_L);
	keymap["SHIFT_L"]       = keysymToKeycode(XK_Shift_L);
	keymap["SHIFT_R"]       = keysymToKeycode(XK_Shift_R);
	keymap["CONTROL"]       = keysymToKeycode(XK_Control_L);
	keymap["CONTROL_L"]     = keysymToKeycode(XK_Control_L);
	keymap["CONTROL_R"]     = keysymToKeycode(XK_Control_R);
	keymap["ALT"]           = keysymToKeycode(XK_Alt_L);
	keymap["ALT_L"]         = keysymToKeycode(XK_Alt_L);
	keymap["ALT_R"]         = keysymToKeycode(XK_Alt_R);
	keymap["CAPSLOCK"]      = keysymToKeycode(XK_Caps_Lock);
	keymap["NUMLOCK"]       = keysymToKeycode(XK_Num_Lock);
	keymap["SCROLLLOCK"]    = keysymToKeycode(XK_Scroll_Lock);
	keymap["KP_MULTIPLY"]   = keysymToKeycode(XK_KP_Multiply);
	keymap["KP_DIVIDE"]     = keysymToKeycode(XK_KP_Divide);
	keymap["KP_SUBTRACT"]   = keysymToKeycode(XK_KP_Subtract);
	keymap["KP_ADD"]        = keysymToKeycode(XK_KP_Add);
	keymap["KP_PERIOD"]     = keysymToKeycode(XK_KP_Separator); // period or comma on keypad
	keymap["SUPER"]         = keysymToKeycode(XK_Super_L);
	keymap["SUPER_L"]       = keysymToKeycode(XK_Super_L);
	keymap["SUPER_R"]       = keysymToKeycode(XK_Super_R);
	keymap["MENU"]          = keysymToKeycode(XK_Menu);

	keymap["BACK"]          = keysymToKeycode(XF86XK_Back);     // browser back
	keymap["FORWARD"]       = keysymToKeycode(XF86XK_Forward);  // browser forward
	keymap["VOLUMEDOWN"]    = keysymToKeycode(XF86XK_AudioLowerVolume);
	keymap["VOLUMEUP"]      = keysymToKeycode(XF86XK_AudioRaiseVolume);
	keymap["MEDIAPLAY"]     = keysymToKeycode(XF86XK_AudioPlay);
	keymap["MEDIASTOP"]     = keysymToKeycode(XF86XK_AudioStop);
	keymap["MEDIAPREVIOUS"] = keysymToKeycode(XF86XK_AudioPrev);
	keymap["MEDIANEXT"]     = keysymToKeycode(XF86XK_AudioNext);

	keymap["SPACE"]         = keysymToKeycode(XK_space);
	keymap["EXCLAM"]        = keysymToKeycode(XK_exclam);       // !
	keymap["QUOTEDBL"]      = keysymToKeycode(XK_quotedbl);     // "
	keymap["NUMBERSIGN"]    = keysymToKeycode(XK_numbersign);   // #
	keymap["DOLLAR"]        = keysymToKeycode(XK_dollar);       // $
	keymap["PERCENT"]       = keysymToKeycode(XK_percent);      // %
	keymap["AMPERSAND"]     = keysymToKeycode(XK_ampersand);    // &
	keymap["APOSTROPHE"]    = keysymToKeycode(XK_apostrophe);   // '
	keymap["PARENLEFT"]     = keysymToKeycode(XK_parenleft);    // (
	keymap["PARENRIGHT"]    = keysymToKeycode(XK_parenright);   // )
	keymap["ASTERISK"]      = keysymToKeycode(XK_asterisk);     // *
	keymap["PLUS"]          = keysymToKeycode(XK_plus);         // +
	keymap["COMMA"]         = keysymToKeycode(XK_comma);        // ,
	keymap["MINUS"]         = keysymToKeycode(XK_minus);        // -
	keymap["PERIOD"]        = keysymToKeycode(XK_period);       // .
	keymap["SLASH"]         = keysymToKeycode(XK_slash);        // /
	keymap["BACKSLASH"]     = keysymToKeycode(XK_backslash);    // \    -

	keymap["COLON"]         = keysymToKeycode(XK_colon);        // :
	keymap["SEMICOLON"]     = keysymToKeycode(XK_semicolon);    // ;
	keymap["LESS"]          = keysymToKeycode(XK_less);         // <
	keymap["EQUAL"]         = keysymToKeycode(XK_equal);        // =
	keymap["GREATER"]       = keysymToKeycode(XK_greater);      // >
	keymap["QUESTION"]      = keysymToKeycode(XK_question);     // ?
	keymap["AT"]            = keysymToKeycode(XK_at);           // @

	keymap["BRACKETLEFT"]   = keysymToKeycode(XK_bracketleft);  // [
	keymap["BRACKETRIGHT"]  = keysymToKeycode(XK_bracketright); // ]
	keymap["BRACELEFT"]     = keysymToKeycode(XK_braceleft);    // {
	keymap["BRACERIGHT"]    = keysymToKeycode(XK_braceright);   // }

	keymap["ASCIICIRCUM"]   = keysymToKeycode(XK_asciicircum);  // ^ character
	keymap["ASCIITILDE"]    = keysymToKeycode(XK_asciitilde);   // ~ character
	keymap["QUOTELEFT"]     = keysymToKeycode(XK_quoteleft);    // ` character
	keymap["ASCIIGRAVE"]    = keysymToKeycode(XK_quoteleft);    // ` character

	// dead keys for accents
	keymap["ACUTE"]         = keysymToKeycode(XK_dead_acute);   // ´
	keymap["CEDILLA"]       = keysymToKeycode(XK_dead_cedilla); // ¸
	keymap["CIRCUM"]        = keysymToKeycode(XK_dead_circumflex); // ^
	keymap["DIAERSIS"]      = keysymToKeycode(XK_dead_diaeresis);  // ¨
	keymap["TILDE"]         = keysymToKeycode(XK_dead_tilde);   // ~
	keymap["GRAVE"]         = keysymToKeycode(XK_dead_grave);   // `
	
	// localisation
	keymap["ARING"]         = keysymToKeycode(XK_Aring);        // Å
	keymap["ADIAERSIS"]     = keysymToKeycode(XK_Adiaeresis);   // Ä
	keymap["ODIAERSIS"]     = keysymToKeycode(XK_Odiaeresis);   // Ö

	keymap["AE"]            = keysymToKeycode(XK_AE);           // Æ
	keymap["OOBLIQUE"]      = keysymToKeycode(XK_Ooblique);     // Ø

	keymap["UDIAERSIS"]     = keysymToKeycode(XK_Udiaeresis);   // Ü
	keymap["SSHARP"]        = keysymToKeycode(XK_ssharp);       // ß

#else //windows
/*
	Same name in windows (omitted here):
	A - Z
	1 - 9
*/
	keymap["ESCAPE"]        = VK_ESCAPE;
	keymap["TAB"]           = VK_TAB;
	keymap["BACKSPACE"]     = VK_BACK;
	keymap["ENTER"]         = VK_RETURN;
	keymap["KP_ENTER"]      = VK_RETURN; // should probably be different
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
	keymap["BACKSLASH"]     = 0;

	keymap["COLON"]         = 0;
	keymap["SEMICOLON"]     = 0;
	keymap["LESS"]          = 0;
	keymap["EQUAL"]         = 0;
	keymap["GREATER"]       = 0;
	keymap["QUESTION"]      = 0;
	keymap["AT"]            = 0;

	keymap["BRACKETLEFT"]   = 0;
	keymap["BRACKETRIGHT"]  = 0;
	keymap["BRACELEFT"]     = 0;
	keymap["BRACERIGHT"]    = 0;

	keymap["ASCIICIRCUM"]   = 0;
	keymap["ASCIITILDE"]    = 0;
	keymap["QUOTELEFT"]     = 0;
	keymap["ASCIIGRAVE"]    = 0;

	// dead keys for accents
	keymap["ACUTE"]         = 0;
	keymap["CEDILLA"]       = 0;
	keymap["CIRCUM"]        = 0;
	keymap["DIAERSIS"]      = 0;
	keymap["TILDE"]         = 0;
	keymap["GRAVE"]         = 0;
	
	// localisation
	keymap["ARING"]         = 0;
	keymap["ADIAERSIS"]     = 0;
	keymap["ODIAERSIS"]     = 0;

	keymap["AE"]            = 0;
	keymap["OOBLIQUE"]      = 0;

	keymap["UDIAERSIS"]     = 0;
	keymap["SSHARP"]        = 0;

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
