@echo off
:: --- PATH CONFIGURATION ---
SET SDK_PATH=C:/SDK
SET SDL_CORE=%SDK_PATH%/SDL2-2.32.6/x86_64-w64-mingw32
SET SDL_IMG=%SDK_PATH%/SDL2_image-2.0.5/x86_64-w64-mingw32
SET SDL_TTF=%SDK_PATH%/SDL2_ttf-2.22.0/x86_64-w64-mingw32

:: --- COMPILATION ---
echo Building Midgard...

:: Added /SDL2 to the end of the -I paths below
g++ -std=c++17 *.cpp -o game.exe ^
-I"%SDL_CORE%/include/SDL2" -I"%SDL_IMG%/include/SDL2" -I"%SDL_TTF%/include/SDL2" ^
-L"%SDL_CORE%/lib" -L"%SDL_IMG%/lib" -L"%SDL_TTF%/lib" ^
-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf ^
-mwindows

if %errorlevel% neq 0 (
    echo.
    echo [!] Compilation failed! Check the errors above.
    pause
    exit /b %errorlevel%
)

echo [!] Build Successful! Starting game...
start game.exe