// =====================================================================
// Input.hpp -- keyboard -> Action                       (PROVIDED, visual)
//
// The game logic never reads the keyboard. Instead, every frame the
// keyboard is turned into a small Action ("move left? shoot?"), and the
// Action is handed to Game::update(). That keeps the logic testable
// without a window -- exactly how Elice tests your PAs.
//
// You do not need to change this file for PA1-PA4.
// =====================================================================
#pragma once
#include <SFML/Graphics.hpp>
#include "Action.hpp"

class Input {
public:
    // Call for every window event: remembers which keys are held down.
    void handleEvent(const sf::Event& event);

    // The Action for this frame, from the keys that are held down now.
    Action currentAction() const;

private:
    // We track key presses/releases ourselves (instead of asking the
    // keyboard directly), which works on every system without extra
    // permissions.
    bool leftHeld_ = false;
    bool rightHeld_ = false;
    bool shootHeld_ = false;
};
