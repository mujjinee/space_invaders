// =====================================================================
// PA1.cpp -- Player and Projectiles                      (YOU WRITE THIS)
//
// Read PA1.hpp first: it lists every number you need (speeds, sizes,
// edges) as a named constant. Use the names, e.g. PLAYER_SPEED, not 330.
//
// Work TODO by TODO (5 TODOs). After each TODO, run its test group:
//     TODO 1-2 -> group 1    TODO 3 -> group 2
//     TODO 4   -> group 3    TODO 5 -> group 4
// The Weapon class at the bottom is provided: read it, do not change it.
// Do not add main(), printing, or SFML code to this file.
// =====================================================================
#include "PA1.hpp"

// ===================== TODO 1: Player constructor =====================
// A new player starts at x = PLAYER_START_X with START_LIVES lives.
// (Set the two private fields x_ and lives_.)
Player::Player() : x_(PLAYER_START_X), lives_(START_LIVES) {}

// ===================== TODO 2: Player::move ===========================
// direction: -1 = left, 0 = stay, +1 = right.   dt: seconds since last frame.
//   1. If direction is greater than 1, use 1. If it is less than -1, use -1.
//   2. If dt is negative, use 0 (time never runs backwards).
//   3. distance = direction * PLAYER_SPEED * dt.  Add it to x_.
//   4. Keep x_ inside [PLAYER_MIN_X, PLAYER_MAX_X]: if it is smaller than
//      PLAYER_MIN_X set it to PLAYER_MIN_X; same idea for PLAYER_MAX_X.
// Example: x_ = 375, move(1, 0.1)  ->  x_ = 375 + 1 * 330 * 0.1 = 408.
void Player::move(int direction, float dt) {
    if (direction > 1) direction = 1;
    if (direction < -1) direction = -1;
    if (dt < 0.0f) dt = 0.0f;

    x_ += direction * PLAYER_SPEED * dt;
    if (x_ < PLAYER_MIN_X) x_ = PLAYER_MIN_X;
    if (x_ > PLAYER_MAX_X) x_ = PLAYER_MAX_X;
}

// ===================== TODO 3: damage, alive, bounds ==================
// takeDamage: lose one life, but lives_ never goes below 0.
void Player::takeDamage() {
    if (lives_ > 0) --lives_;
}

// isAlive: true while the player has at least one life.
bool Player::isAlive() const {
    return lives_ > 0;
}

// bounds: the ship's rectangle. Its top-left corner is (x_, PLAYER_Y),
// its size is PLAYER_W x PLAYER_H.   Hint: return {x_, PLAYER_Y, ..., ...};
// Why? Nothing in PA1 uses it yet. The game draws the ship inside this
// rectangle, and in PA3 a hit is "do two rectangles overlap?":
//     overlaps(shot.bounds(), player.bounds())
Rect Player::bounds() const {
    return {x_, PLAYER_Y, PLAYER_W, PLAYER_H};
}

// ===================== TODO 4: Projectile =============================
// Constructor: store x, y and friendly in x_, y_ and friendly_.
// A new projectile is alive (alive_ = true).
Projectile::Projectile(float x, float y, bool friendly)
    : x_(x), y_(y), friendly_(friendly), alive_(true) {}

// update: move the shot for dt seconds.
//   1. If the shot is already dead, or dt is negative, do nothing (return).
//   2. A friendly shot moves UP:   y_ = y_ - PLAYER_SHOT_SPEED * dt.
//      An enemy shot moves DOWN:   y_ = y_ + ENEMY_SHOT_SPEED * dt.
//   3. If y_ < SHOT_MIN_Y or y_ > SHOT_MAX_Y, the shot has left the
//      screen: set alive_ to false.
// Example: friendly shot at y = 100, update(0.1) -> y = 100 - 44 = 56.
void Projectile::update(float dt) {
    if (!alive_ || dt < 0.0f) return;

    if (friendly_) {
        y_ -= PLAYER_SHOT_SPEED * dt;
    } else {
        y_ += ENEMY_SHOT_SPEED * dt;
    }
    if (y_ < SHOT_MIN_Y || y_ > SHOT_MAX_Y) alive_ = false;
}

// bounds: top-left (x_, y_), size SHOT_W x SHOT_H. Same idea as Player::bounds:
// the box the game draws, and the box PA3 checks for hits.
Rect Projectile::bounds() const {
    return {x_, y_, SHOT_W, SHOT_H};
}

// ===================== TODO 5: FireControl ============================
// remaining_ = seconds to wait before the next shot (0 means ready).
// update: if dt is positive, subtract it from remaining_.
//         remaining_ never goes below 0.
void FireControl::update(float dt) {
    if (dt <= 0.0f) return;
    remaining_ -= dt;
    if (remaining_ < 0.0f) remaining_ = 0.0f;
}

// tryShoot: "may I shoot now?"
//   - still waiting (remaining_ > 0): answer false and change nothing;
//   - ready: start a new cooldown (remaining_ = FIRE_COOLDOWN), answer true.
bool FireControl::tryShoot() {
    if (remaining_ > 0.0f) return false;
    remaining_ = FIRE_COOLDOWN;
    return true;
}

// ===================== PROVIDED: Weapon (read it, do not change it) ====
// Weapon is already written for you. Read it: it shows two W11 ideas that
// you will write yourself in PA3.
// shots_ is a vector<Projectile> holding every shot still in the game.

// fire: add a new Projectile(x, y, friendly) to the end of shots_.
void Weapon::fire(float x, float y, bool friendly) {
    shots_.push_back(Projectile(x, y, friendly));
}

// updateAll: call update(dt) on EVERY shot in shots_.
// The & matters: "Projectile& shot" is the real shot in the vector.
// Without it, shot would be a copy -- the copy would move, the real one not.
void Weapon::updateAll(float dt) {
    for (Projectile& shot : shots_) {
        shot.update(dt);
    }
}

// removeDead: keep only the shots that are still alive, in the same order.
// Build a new vector of survivors, then replace shots_ with it.
void Weapon::removeDead() {
    std::vector<Projectile> survivors;
    for (const Projectile& shot : shots_) {
        if (shot.alive()) survivors.push_back(shot);
    }
    shots_ = survivors;
}
