#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <SDL3/SDL.h>
#include <functional>
#include <vector>
#include "ui/Menu.h"

class EventBus {
public:
    using QuitCallback = std::function<void()>;
    using KeyDownCallback = std::function<void(SDL_Keycode key, bool repeat)>;
    using KeyboardStateCallback = std::function<void(const bool* keys)>;
    using GamepadStateCallback = std::function<void(SDL_Gamepad* gamepad)>;
    using GamepadButtonDownCallback = std::function<void(Uint8 button)>;
    using MenuOptionSelectedCallback = std::function<void(MenuOption option)>;

    void subscribeQuitRequested(QuitCallback cb);
    void publishQuitRequested();

    void subscribeKeyDown(KeyDownCallback cb);
    void publishKeyDown(SDL_Keycode key, bool repeat);

    void subscribeKeyboardState(KeyboardStateCallback cb);
    void publishKeyboardState(const bool* keys);

    void subscribeGamepadState(GamepadStateCallback cb);
    void publishGamepadState(SDL_Gamepad* gamepad);

    void subscribeGamepadButtonDown(GamepadButtonDownCallback cb);
    void publishGamepadButtonDown(Uint8 button);

    void subscribeMenuOptionSelected(MenuOptionSelectedCallback cb);
    void publishMenuOptionSelected(MenuOption option);

private:
    std::vector<QuitCallback> quitSubscribers;
    std::vector<KeyDownCallback> keyDownSubscribers;
    std::vector<KeyboardStateCallback> keyboardStateSubscribers;
    std::vector<GamepadStateCallback> gamepadStateSubscribers;
    std::vector<GamepadButtonDownCallback> gamepadButtonDownSubscribers;
    std::vector<MenuOptionSelectedCallback> menuOptionSelectedSubscribers;
};

#endif
