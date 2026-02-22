
Welcome to the Midgard repository. To ensure we don't spend 4 hours debugging "File Not Found" errors, please follow these setup instructions exactly.

🛠 Prerequisites & Setup
We are using MinGW-w64 (via **MSYS2**). Make sure your compiler is 64-bit to match the libraries.

**Lib instal:**
Download the Development Libraries (eg: SDL2-devel-2.32.6-mingw.zip) for the following. Use the x86_64-w64-mingw32 versions:

SDL2 (v2.32.6)

SDL2_ttf (v2.22.0)

SDL2_image (v2.8.2) 

**Universal Paths**

To keep our build.bat working for everyone, do not point to other folders on your drive.

Create a folder at C:/ called SDK/

Extract your SDLS there so the paths look like this:

C:/SDK/SDL2/

C:/SDK/SDL2_ttf/

C:/SDK/SDL2_image/

Make sure you run this in mysys2:

pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-make

this installs your programs to your msys2.


to build, just run the bat file

Note: If you get an "Undefined Reference" error, ensure your C:/SDK paths are correctly set in the script and you installed with the pacman commands into msys2

to avoid conflicts, heres what is being actively worked on:
Zakk: Fullscreen, UI, path fixing.
