# gdvk - Godot Virtual Keyboard
Send virtual keyboard presses in Godot on Linux(X11)/Windows

On Windows a lot of special characters/keys are missing but on Linux most of what you need should be there. The Windows virtual keycodes are [here](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes).

Please inform me about how my code is shit; I'm new to C++.

# Usage
Keys mostly use the [same naming as Godot](https://docs.godotengine.org/en/stable/classes/class_%40globalscope.html) minus the KEY_ prefix. Note that GDVK does not support all keys that Godot supports but some that Godot does not, read the source code if you need to (the key map is at the end of gdvk.cpp). 

# Compiling
You will need scons installed and for cross compiling for windows you need `mingw-w64-gcc`
```
git submodule update --init --recursive
cd godot-cpp
scons platform=<PLATFORM> generate_bindings=yes target=release -j12
cd ..
scons platform=<PLATFORM> target=release
```
Change `<PLATFORM>` to `linux`/`windows` and the 12 in `-j12` to the number of threads on your system.
You only need to run the first scons command once per platform.
Remove `target=release` from both commands if you need debugging, but this makes the binaries way bigger and slower.
