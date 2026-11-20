// =====================================================================
// main.cpp -- window and game loop                     (PROVIDED, visual)
//
// WEEK 12 version. Every frame (60 times a second):
//     1. read window events (keys, close button)
//     2. game.update(action, dt)   <- your PA1 / PA2 code runs here
//     3. draw everything           <- only READS your objects
//
// Keys: Left/Right or A/D move, Space shoots,
//       R = restart, H = lose a life (test), Esc = quit.
// You do not need to change this file for PA1-PA2.
// =====================================================================
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Input.hpp"
#include "Draw.hpp"
#include "PixelFont.hpp"
#include <cstdlib>   // std::srand
#include <ctime>     // std::time

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));   // different enemy shots each game
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "CP2 Space Invaders - Week 12");
    window.setFramerateLimit(60);       // at most 60 frames per second
    window.setKeyRepeatEnabled(false);  // a held key sends ONE KeyPressed event

    Game game;
    Input input;
    sf::Clock clock;    // measures the time between two frames

    while (window.isOpen()) {
        // ---- 1. events --------------------------------------------------
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            input.handleEvent(*event);   // remembers held keys for the Action
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) window.close();
                if (key->code == sf::Keyboard::Key::R) game.restart();
                if (key->code == sf::Keyboard::Key::H) game.testHit();
            }
        }
        if (!window.isOpen()) break;

        // ---- 2. update -------------------------------------------------
        // dt = seconds since the last frame. If the window was dragged or
        // paused, dt can be huge, so we cap it at 0.05 s.
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;
        game.update(input.currentAction(), dt);

        // ---- 3. draw ---------------------------------------------------
        drawBackground(window);
        drawFleet(window, game.fleet());              // PA2
        drawShots(window, game.playerShots());        // PA1
        drawShots(window, game.enemyShots());         // PA1
        drawPlayer(window, game.player(), true);      // PA1
        drawHud(window, 0, game.player().lives(), 0, game.paStatus());
        drawText(window, "R RESTART  H TEST HIT", 250.0f, 576.0f, 2.0f, COLOR_LINE);
        window.display();   // show the finished frame
    }
    return 0;
}
