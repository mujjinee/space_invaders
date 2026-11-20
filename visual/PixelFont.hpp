// =====================================================================
// PixelFont.hpp -- draws text with small squares       (PROVIDED, visual)
//
// The game needs no font files: every letter is a 5 x 7 grid of pixels,
// and each "on" pixel is drawn as a small filled square.
// You do not need to change this file for PA1-PA4.
// =====================================================================
#pragma once
// sf::RenderTarget& window = the window (or any other surface SFML can draw on).
#include <SFML/Graphics.hpp>
#include <string>

// Draw a filled rectangle. Every picture in the game is made of these.
void drawBox(sf::RenderTarget& window, float x, float y,
             float width, float height, sf::Color color);

// Draw text with its top-left corner at (x, y).
// scale = size of one font pixel, in screen pixels (2.0f is small, 4.0f is big).
// Letters are drawn in UPPER CASE; unknown characters are drawn as blanks.
void drawText(sf::RenderTarget& window, const std::string& text,
              float x, float y, float scale, sf::Color color);

// Width of a text in screen pixels (useful for centering).
float textWidth(const std::string& text, float scale);
