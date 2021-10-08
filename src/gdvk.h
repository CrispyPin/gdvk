#ifndef GDVK_H
#define GDVK_H

#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>

#include <unordered_map>
#include <string>

#ifdef __linux__
#include <unistd.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#else
#include <windows.h>
#endif

using namespace std;

namespace godot {

class GDVK : public Node {
    GODOT_CLASS(GDVK, Node)

private:
#ifdef __linux__
    Display* xdisplay;
#endif
    void setKeyState(const String, bool);
    void delay(unsigned int);

    unordered_map<string, unsigned int> keymap;
    void generateKeymap();

    unsigned long stringToKeyCode(const char*);
    unsigned long lookupKeyCode(const String);

public:
    static void _register_methods();

    GDVK();
    ~GDVK();

    void _init();
    void keyDown (const String);
    void keyUp   (const String);
    void keyPress(const String);
};

}

#endif // GDVK_H