// =====================================================================
// Input.cpp -- keyboard -> Action                       (PROVIDED, visual)
//
// Controls:  Left / A = move left,  Right / D = move right,  Space = shoot.
// You do not need to change this file for PA1-PA4.
// =====================================================================
#include "Input.hpp"

void Input::handleEvent(const sf::Event& event) {
    // If the window loses focus we never see the key being released,
    // so forget every held key.
    if (event.is<sf::Event::FocusLost>()) {
        leftHeld_ = false;
        rightHeld_ = false;
        shootHeld_ = false;
    }

    // A key went down: remember it.
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) leftHeld_ = true;
        if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D) rightHeld_ = true;
        if (key->code == sf::Keyboard::Key::Space) shootHeld_ = true;
    }

    // A key went up: forget it.
    if (const auto* key = event.getIf<sf::Event::KeyReleased>()) {
        if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) leftHeld_ = false;
        if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D) rightHeld_ = false;
        if (key->code == sf::Keyboard::Key::Space) shootHeld_ = false;
    }
}

Action Input::currentAction() const {
    Action action;
    if (leftHeld_) action.move -= 1;    // both held: -1 + 1 = 0, stay still
    if (rightHeld_) action.move += 1;
    action.shoot = shootHeld_;
    return action;
}
