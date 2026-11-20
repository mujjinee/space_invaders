// =====================================================================
// Draw.hpp -- draws the game objects                    (PROVIDED, visual)
//
// Every function here only READS your objects through their const
// getters (x(), bounds(), alive(), type(), ...). Drawing never changes
// the game. So: if your logic is right, the picture is right.
//
// You do not need to change this file for PA1-PA4.
// =====================================================================
#pragma once
// sf::RenderTarget& window = the window (or any other surface SFML can draw on).
#include <SFML/Graphics.hpp>
#include <string>
#include "PA1.hpp"
#include "PA2.hpp"

// The colors of the game (red, green, blue from 0 to 255).
const sf::Color COLOR_BACKGROUND(8, 13, 29);
const sf::Color COLOR_STAR(54, 75, 112);
const sf::Color COLOR_LINE(41, 69, 99);
const sf::Color COLOR_CYAN(77, 219, 248);     // the ship and player shots
const sf::Color COLOR_GOLD(255, 207, 94);     // enemy shots, armored enemies
const sf::Color COLOR_RED(255, 102, 125);     // red enemies
const sf::Color COLOR_PURPLE(173, 121, 255);  // normal enemies
const sf::Color COLOR_PALE(210, 229, 246);    // HUD labels
const sf::Color COLOR_GREEN(120, 230, 140);
const sf::Color COLOR_PANEL(18, 28, 49);      // message boxes

// Clear the window: dark background, stars, two thin lines.
void drawBackground(sf::RenderTarget& window);

// The ship (PA1). Reads player.bounds(). visible == false hides it
// (the game blinks the ship for a moment after it is hit).
void drawPlayer(sf::RenderTarget& window, const Player& player, bool visible);

// Every ALIVE projectile of a weapon (PA1). Reads shot.alive(), shot.bounds(),
// shot.friendly(). Player shots are cyan, enemy shots gold.
void drawShots(sf::RenderTarget& window, const Weapon& weapon);

// Every ALIVE enemy of the fleet (PA2). Reads enemy->type() to pick the
// picture and color, and enemy->bounds() for the position and size.
void drawFleet(sf::RenderTarget& window, const Fleet& fleet);

// The top and bottom bars: title, score, lives, best score, and a small
// line showing which PAs the game detected as done.
void drawHud(sf::RenderTarget& window, int score, int lives, int best,
             const std::string& paStatus);

// A box in the middle of the screen with a big and a small line of text.
void drawMessage(sf::RenderTarget& window, const std::string& big,
                 sf::Color bigColor, const std::string& small);
