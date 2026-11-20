// =====================================================================
// common.hpp -- shared by every PA and by the game. (PROVIDED)
//
// Screen coordinates, in pixels:
//
//     (0,0) ------------------------> x        (800, 0)
//       |
//       |      smaller y = higher on the screen
//       v
//       y                                      (800, 600)
//
// Do not change this file.
// =====================================================================
#pragma once   // include this file only once per .cpp, even if asked twice

// Size of the game window, in pixels.
const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

// A rectangle: top-left corner (x, y), width w and height h, in pixels.
// Every object in the game (ship, shot, enemy) is a rectangle.
struct Rect {
    float x;
    float y;
    float w;
    float h;
};
