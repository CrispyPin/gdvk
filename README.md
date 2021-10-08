# gdvk - Godot Virtual Keyboard
Send virtual keyboard presses in godot on linux/windows

# Compiling
```
cd godot-cpp
scons platform=<PLATFORM> generate_bindings=yes -j12
cd ..
scons platform=<PLATFORM>
```
Change `<PLATFORM>` to `linux`/`windows` and the 12 in `-j12` to the number of threads on your system.
Add `target=release` to both commands if you're not just testing, this makes the binary a lot smaller and faster.
