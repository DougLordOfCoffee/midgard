#include "event_bus/EventBus.h"

void EventBus::subscribeQuitRequested(QuitCallback cb) {
    quitSubscribers.push_back(std::move(cb));
}

void EventBus::publishQuitRequested() {
    for (auto& cb : quitSubscribers) {
        cb();
    }
}

void EventBus::subscribeKeyDown(KeyDownCallback cb) {
    keyDownSubscribers.push_back(std::move(cb));
}

void EventBus::publishKeyDown(SDL_Keycode key, bool repeat) {
    for (auto& cb : keyDownSubscribers) {
        cb(key, repeat);
    }
}

void EventBus::subscribeKeyboardState(KeyboardStateCallback cb) {
    keyboardStateSubscribers.push_back(std::move(cb));
}

void EventBus::publishKeyboardState(const bool* keys) {
    for (auto& cb : keyboardStateSubscribers) {
        cb(keys);
    }
}

void EventBus::subscribeGamepadState(GamepadStateCallback cb) {
    gamepadStateSubscribers.push_back(std::move(cb));
}

void EventBus::publishGamepadState(SDL_Gamepad* gamepad) {
    for (auto& cb : gamepadStateSubscribers) {
        cb(gamepad);
    }
}

void EventBus::subscribeGamepadButtonDown(GamepadButtonDownCallback cb) {
    gamepadButtonDownSubscribers.push_back(std::move(cb));
}

void EventBus::publishGamepadButtonDown(Uint8 button) {
    for (auto& cb : gamepadButtonDownSubscribers) {
        cb(button);
    }
}

void EventBus::subscribeMenuOptionSelected(MenuOptionSelectedCallback cb) {
    menuOptionSelectedSubscribers.push_back(std::move(cb));
}

void EventBus::publishMenuOptionSelected(MenuOption option) {
    for (auto& cb : menuOptionSelectedSubscribers) {
        cb(option);
    }
}
