@echo off
D:\mysys2\mingw64\bin\g++.exe -std=c++17 main.cpp Player.cpp Camera.cpp Layer.cpp Chunk.cpp TileMap.cpp NoiseGenerator.cpp TextureManager.cpp TextRenderer.cpp WorldLoader.cpp WorldImporter.cpp Minimap.cpp Menu.cpp -o game -LD:\SDL2-2.32.6\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -ID:\SDL2-2.32.6\x86_64-w64-mingw32\include -IC:\msys64\mingw64\include -IC:\msys64\mingw64\include\SDL2 -LD:\SDL2-2.32.6\x86_64-w64-mingw32\lib -LC:\msys64\mingw64\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf 
if %errorlevel% equ 0 (
    echo Compilation successful! 
    game.exe
) else (
    echo Compilation failed!
    pause
)
