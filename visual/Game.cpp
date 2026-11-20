// =====================================================================
// Game.cpp -- connects your PA objects into one game   (PROVIDED)
//
// WEEK 12 version (PA1 + PA2). Read update() below: it is the heart of
// the game. Every frame (60 times a second) it calls YOUR functions in a
// fixed order. Comments say which PA each call comes from.
//
// You do not need to change this file for PA1-PA2.
// =====================================================================
#include "Game.hpp"
#include <cstdlib>   // std::rand

namespace {
const float ENEMY_FIRE_INTERVAL = 1.0f;   // an enemy shoots every 1.0 second

// Does PA1 look done? A finished Player starts at x = 375 with 3 lives.
bool pa1Done() {
    Player test;
    return test.lives() == START_LIVES && test.x() > 374.0f && test.x() < 376.0f;
}

// Does PA2 look done? A finished Fleet::build makes 36 enemies.
bool pa2Done() {
    Fleet test;
    test.build(FORMATION, FORMATION_ROWS);
    return test.count() == 36;
}
}  // namespace

Game::Game() {
    fleet_.build(FORMATION, FORMATION_ROWS);   // PA2
    paStatus_ = std::string("PA1 ") + (pa1Done() ? "OK" : "--")
              + "  PA2 " + (pa2Done() ? "OK" : "--");
}

void Game::update(Action action, float dt) {
    // 1. Move the ship.                                              (PA1)
    player_.move(action.move, dt);

    // 2. Shoot: the cooldown timer decides if a new shot is allowed. (PA1)
    //    The shot starts at the middle of the ship's top edge.
    fire_.update(dt);
    if (action.shoot && fire_.tryShoot()) {
        float shotX = player_.x() + PLAYER_W / 2.0f - SHOT_W / 2.0f;
        playerShots_.fire(shotX, PLAYER_Y - SHOT_H, true);
    }

    // 3. Move the fleet: sideways, and down at each wall.            (PA2)
    fleet_.update(dt);

    // 4. Now and then a random enemy shoots.        (uses PA1 and PA2)
    enemiesShoot(dt);

    // 5. Move every shot, then throw away shots that left the screen. (PA1)
    playerShots_.updateAll(dt);
    enemyShots_.updateAll(dt);
    playerShots_.removeDead();
    enemyShots_.removeDead();

    // Week 12 only: there is no game over yet (that is PA4), so when the
    // fleet reaches the ship we simply start a new fleet.
    if (fleet_.count() > 0 && fleet_.lowestY() >= PLAYER_Y) {
        fleet_.build(FORMATION, FORMATION_ROWS);
    }
    // Week 13 adds here: collisions and score (PA3), win / lose (PA4).
}

void Game::enemiesShoot(float dt) {
    enemyFireTimer_ -= dt;
    if (enemyFireTimer_ > 0.0f || fleet_.count() == 0) return;
    enemyFireTimer_ = ENEMY_FIRE_INTERVAL;

    // pick a random enemy; it shoots from the middle of its bottom edge
    const Enemy& shooter = *fleet_.enemies()[std::rand() % fleet_.count()];
    float shotX = shooter.x() + ENEMY_W / 2.0f - SHOT_W / 2.0f;
    enemyShots_.fire(shotX, shooter.y() + ENEMY_H, false);
}

void Game::restart() {
    player_ = Player();
    fire_ = FireControl();
    playerShots_ = Weapon();
    enemyShots_ = Weapon();
    fleet_.build(FORMATION, FORMATION_ROWS);
    enemyFireTimer_ = 1.0f;
}

void Game::testHit() {
    player_.takeDamage();   // PA1
}
