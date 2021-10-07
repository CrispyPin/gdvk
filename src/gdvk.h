#ifndef GDVK_H
#define GDVK_H

#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>

#ifdef __linux__
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#else
#include <windows.h>
//#include <unordered_map>
#endif


namespace godot {

class GDVK : public Node {
    GODOT_CLASS(GDVK, Node)

private:
#ifdef __linux__
    Display* xdisplay;
    char stringToKeyCode(const String);
#else
    DWORD stringToKeyCode(const String);
#endif
    void setKeyState(const String, bool);

public:
    static void _register_methods();

    GDVK();
    ~GDVK();

    void _init(); // initializer called by Godot
    void keyDown (const String);
    void keyUp   (const String);
    void keyPress(const String);
};

}

#endif // GDVK_H