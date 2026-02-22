@echo off
D:\mysys2\mingw64\bin\g++.exe -std=c++17 main.cpp Player.cpp Camera.cpp Layer.cpp -o game -LD:\SDL2-2.32.6\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 -ID:\SDL2-2.32.6\x86_64-w64-mingw32\include 
if %errorlevel% equ 0 (
    echo Compilation successful!
    game.exe
) else (
    echo Compilation failed!
    pause
)
