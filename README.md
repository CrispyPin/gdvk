# gdvk - Godot Virtual Keyboard
Send virtual keyboard presses in godot on linux/windows

Please inform me about how my code is shit; I'm new to C++.

# Compiling
```
cd godot-cpp
scons platform=<PLATFORM> generate_bindings=yes -j12
cd ..
scons platform=<PLATFORM>
```
Change `<PLATFORM>` to `linux`/`windows` and the 12 in `-j12` to the number of threads on your system.
Add `target=release` to both commands if you're not just testing, this makes the binary a lot smaller and faster.

# Usage
It's not done yet but sure

All the special keys (like modifiers) use the [same naming as godot](https://docs.godotengine.org/en/stable/classes/class_%40globalscope.html) minus the KEY_ prefix.
