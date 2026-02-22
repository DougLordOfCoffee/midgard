@echo off
:: --- PATH CONFIGURATION (MSYS2 MinGW64 - install via: pacman -S mingw-w64-x86_64-sdl3 mingw-w64-x86_64-sdl3-image mingw-w64-x86_64-sdl3-ttf) ---
SET MINGW64=C:/msys64/mingw64
SET INC=%MINGW64%/include
SET LIB=%MINGW64%/lib
:: Prepend MinGW bin to PATH so g++ is found when run from PowerShell/cmd (not just MSYS2 shell)
SET "PATH=%MINGW64%/bin;%PATH%"

:: --- COMPILATION ---
echo Building Midgard...

g++ -std=c++17 -Isrc src/game/*.cpp src/camera/*.cpp src/player/*.cpp src/world/*.cpp src/render/*.cpp src/ui/*.cpp src/event_bus/*.cpp -o game.exe ^
-I"%INC%" -L"%LIB%" ^
-lmingw32 -lSDL3 -lSDL3_image -lSDL3_ttf ^
-mwindows

if %errorlevel% neq 0 (
    echo.
    echo [!] Compilation failed! Check the errors above.
    pause
    exit /b %errorlevel%
)

echo [!] Build Successful! Starting game...
start game.exe