# Midgard

## Setup

1. **Install [MSYS2](https://www.msys2.org/)** and follow its instructions to update.

2. Open **MSYS2 MinGW 64-bit** and install the toolchain and SDL3:

   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-make mingw-w64-x86_64-sdl3 mingw-w64-x86_64-sdl3-image mingw-w64-x86_64-sdl3-ttf
   ```

3. **Build:** run `build.bat` (from a shell where MinGW is on PATH, e.g. MSYS2 MinGW 64-bit).
