// =====================================================================
// PA1.hpp -- Player and Projectiles (PROVIDED, read-only)
//
// You implement the functions marked "PA1.cpp" in PA1.cpp.
// The short getters written inside the classes are already done.
//
// Concepts: class, constructor, private fields (W5),
//           const getters, vector of objects (W11), if / clamping (W2).
// =====================================================================
#pragma once
#include "common.hpp"
#include <vector>

// ---------------- numbers used by PA1 (all in pixels or seconds) -------
const float PLAYER_START_X = 375.0f;   // left edge of the ship at the start
const float PLAYER_Y       = 538.0f;   // top edge of the ship (it never moves up or down)
const float PLAYER_W       = 50.0f;    // ship width
const float PLAYER_H       = 24.0f;    // ship height
const float PLAYER_SPEED   = 330.0f;   // pixels per second
const float PLAYER_MIN_X   = 12.0f;    // the ship's x must stay in [12, 738]
const float PLAYER_MAX_X   = 738.0f;   //   (738 = 800 - 50 - 12)
const int   START_LIVES    = 3;

const float SHOT_W            = 5.0f;    // projectile width
const float SHOT_H            = 14.0f;   // projectile height
const float PLAYER_SHOT_SPEED = 440.0f;  // a player shot moves UP   (y gets smaller)
const float ENEMY_SHOT_SPEED  = 210.0f;  // an enemy shot moves DOWN (y gets larger)
const float SHOT_MIN_Y        = -20.0f;  // a shot with y < -20 or y > 620
const float SHOT_MAX_Y        = 620.0f;  //   has left the screen: it is dead

const float FIRE_COOLDOWN = 0.25f;       // seconds between two player shots

// ---------------------------------------------------------------------
// Player: the ship at the bottom of the screen.
// It only moves left and right, so it stores only its x position.
// ---------------------------------------------------------------------
class Player {
public:
    Player();                              // PA1.cpp  TODO 1
    void move(int direction, float dt);    // PA1.cpp  TODO 2
    void takeDamage();                     // PA1.cpp  TODO 3
    bool isAlive() const;                  // PA1.cpp  TODO 3
    Rect bounds() const;                   // PA1.cpp  TODO 3

    float x() const { return x_; }         // provided
    int lives() const { return lives_; }   // provided

private:
    float x_ = 0.0f;   // left edge of the ship, in pixels
    int lives_ = 0;    // lives left; 0 means game over
};

// ---------------------------------------------------------------------
// Projectile: one shot. friendly == true  -> fired by the player (moves up)
//                       friendly == false -> fired by an enemy  (moves down)
// ---------------------------------------------------------------------
class Projectile {
public:
    Projectile(float x, float y, bool friendly);   // PA1.cpp  TODO 4
    void update(float dt);                         // PA1.cpp  TODO 4
    Rect bounds() const;                           // PA1.cpp  TODO 4

    float x() const { return x_; }                 // provided
    float y() const { return y_; }                 // provided
    bool friendly() const { return friendly_; }    // provided
    bool alive() const { return alive_; }          // provided
    void destroy() { alive_ = false; }             // provided (used in PA3)

private:
    float x_ = 0.0f;         // left edge, in pixels
    float y_ = 0.0f;         // top edge, in pixels
    bool friendly_ = true;   // true: player shot, false: enemy shot
    bool alive_ = false;     // false: this shot can be removed
};

// ---------------------------------------------------------------------
// FireControl: a cooldown timer. It only says YES or NO to "may I shoot?".
// It does not create projectiles -- the Weapon does that.
// ---------------------------------------------------------------------
class FireControl {
public:
    void update(float dt);   // PA1.cpp  TODO 5
    bool tryShoot();         // PA1.cpp  TODO 5

    float remaining() const { return remaining_; }   // provided

private:
    float remaining_ = 0.0f;   // seconds left before the next shot; 0 = ready
};

// ---------------------------------------------------------------------
// Weapon: keeps every projectile that is still flying, in a vector.
// The game has two weapons: one for player shots, one for enemy shots.
// ---------------------------------------------------------------------
class Weapon {
public:
    void fire(float x, float y, bool friendly);   // PA1.cpp  provided (read it)
    void updateAll(float dt);                     // PA1.cpp  provided (read it)
    void removeDead();                            // PA1.cpp  provided (read it)

    int count() const { return static_cast<int>(shots_.size()); }       // provided
    const std::vector<Projectile>& shots() const { return shots_; }     // provided (for drawing)
    std::vector<Projectile>& shots() { return shots_; }                 // provided (for PA3)

private:
    std::vector<Projectile> shots_;   // every shot that is still in the game
};
