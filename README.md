# gdvk - Godot Vitual Keyboard
Send virtual keyboard presses in godot on linux/windows

# Compiling
```
cd godot-cpp
scons platform=<PLATFORM> generate_bindings=yes -j12
cd ..
scons platform=<PLATFORM>
```
Change `<PLATFORM>` to `linux`/`windows` and the 12 in `-j12` to the number of threads on your system.
