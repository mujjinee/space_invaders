// =====================================================================
// Game.hpp -- connects your PA objects into one game   (PROVIDED)
//
// WEEK 12 version: uses PA1 (Player, FireControl, Weapon) and
// PA2 (Fleet). Collisions, score and game over come in Week 13.
//
// Game uses NO SFML: it is plain C++. main.cpp reads the keyboard and
// draws; Game only updates your objects. Read Game.cpp to see, step by
// step, when each of your functions is called.
// =====================================================================
#pragma once
#include "PA1.hpp"
#include "PA2.hpp"
#include "Action.hpp"   // struct Action: what the player wants this frame
#include <string>

class Game {
public:
    Game();

    // One frame of the game: dt seconds have passed, action = the keys.
    void update(Action action, float dt);

    void restart();   // key R: a fresh ship and a fresh fleet
    void testHit();   // key H: the ship loses one life (to test takeDamage)

    // Read access for drawing (main.cpp)
    const Player& player() const { return player_; }
    const Weapon& playerShots() const { return playerShots_; }
    const Weapon& enemyShots() const { return enemyShots_; }
    const Fleet& fleet() const { return fleet_; }

    // e.g. "PA1 OK  PA2 --": which PAs this game detects as done
    std::string paStatus() const { return paStatus_; }

private:
    Player player_;          // PA1
    FireControl fire_;       // PA1
    Weapon playerShots_;     // PA1: shots going up
    Weapon enemyShots_;      // PA1: shots going down
    Fleet fleet_;            // PA2
    float enemyFireTimer_ = 1.0f;   // seconds until an enemy shoots
    std::string paStatus_;

    void enemiesShoot(float dt);
};
