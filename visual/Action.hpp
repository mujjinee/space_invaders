// =====================================================================
// Action.hpp -- what the player wants to do in one frame   (PROVIDED)
//
// The keyboard code (Input.cpp) fills an Action; Game::update() uses it.
// Plain C++, no SFML -- so the game logic never touches the keyboard.
// EXTENSION (agent mode): an AI can fill an Action instead of the keyboard.
// =====================================================================
#pragma once

struct Action {
    int move = 0;         // -1 = left, 0 = stay, +1 = right
    bool shoot = false;   // true while the fire key is held
};
