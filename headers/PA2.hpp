// =====================================================================
// PA2.hpp -- Enemies and the Fleet (PROVIDED, read-only)
//
// You implement the functions marked "PA2.cpp" in PA2.cpp.
//
// Concepts: inheritance (W6), virtual / override (W7),
//           vector<unique_ptr<...>> -- no delete needed (W8),
//           a 2D layout read with nested loops (W4), characters of a string (W10),
//           switch (W2).
// =====================================================================
#pragma once
#include "common.hpp"
#include <memory>    // std::unique_ptr
#include <string>
#include <vector>

// ---------------- numbers used by PA2 (pixels, seconds, points) --------
const float ENEMY_W = 34.0f;          // every enemy is 34 x 24 pixels
const float ENEMY_H = 24.0f;
const float FLEET_LEFT = 92.0f;       // x of the enemy in column 0
const float FLEET_TOP = 92.0f;        // y of the enemy in row 0
const float COLUMN_GAP = 68.0f;       // column c is at x = 92 + c * 68
const float ROW_GAP = 45.0f;          // row r    is at y = 92 + r * 45
const float FLEET_SPEED = 53.0f;      // sideways speed, pixels per second
const float WALL_LEFT = 30.0f;        // the fleet turns round when an enemy reaches
const float WALL_RIGHT = 770.0f;      //   x <= 30 (left) or right edge >= 770 (right)
const float STEP_DOWN = 16.0f;        // pixels the fleet moves down at each turn

const int NORMAL_POINTS = 10;
const int RED_POINTS = 20;
const int ARMORED_POINTS = 30;
const int ARMOR_HITS = 2;             // an armored enemy needs 2 hits

// The starting formation. Each string is one row, each character one column:
//   'R' = RedEnemy, 'A' = ArmoredEnemy, 'N' = normal Enemy, '.' = empty slot.
const int FORMATION_ROWS = 4;
const std::string FORMATION[FORMATION_ROWS] = {
    "RRRRRRRRR",
    "AAAAAAAAA",
    "NNNNNNNNN",
    "NNNNNNNNN",
};

// ---------------------------------------------------------------------
// Enemy: the base class. A normal enemy is worth 10 points and dies
// after one hit.
// ---------------------------------------------------------------------
class Enemy {
public:
    Enemy(float x, float y);              // PA2.cpp  TODO 1
    virtual ~Enemy() {}                   // provided: lets unique_ptr<Enemy> delete
                                          // a RedEnemy or ArmoredEnemy correctly
    void translate(float dx, float dy);   // PA2.cpp  TODO 1
    Rect bounds() const;                  // PA2.cpp  TODO 1

    virtual int points() const;           // PA2.cpp  TODO 2
    virtual char type() const;            // PA2.cpp  TODO 2  ('N', 'R' or 'A')
    virtual void hit();                   // PA2.cpp  TODO 2

    float x() const { return x_; }        // provided
    float y() const { return y_; }        // provided
    bool alive() const { return alive_; } // provided

private:
    float x_ = 0.0f;       // left edge, in pixels
    float y_ = 0.0f;       // top edge, in pixels
    bool alive_ = false;   // false: this enemy has been destroyed
};

// RedEnemy: the top row. Worth 20 points, dies after one hit.
class RedEnemy : public Enemy {
public:
    RedEnemy(float x, float y);           // PA2.cpp  TODO 3
    int points() const override;          // PA2.cpp  TODO 3
    char type() const override;           // PA2.cpp  TODO 3
};

// ArmoredEnemy: worth 30 points. The first hit only breaks its armor;
// the second hit destroys it.
class ArmoredEnemy : public Enemy {
public:
    ArmoredEnemy(float x, float y);       // PA2.cpp  TODO 4
    int points() const override;          // PA2.cpp  TODO 4
    char type() const override;           // PA2.cpp  TODO 4
    void hit() override;                  // PA2.cpp  TODO 4

    int armor() const { return armor_; }  // provided: hits still needed

private:
    int armor_ = 0;
};

// ---------------------------------------------------------------------
// Fleet: owns every enemy. Each enemy lives in a unique_ptr, so when the
// vector removes it (erase or clear) the enemy is deleted automatically.
// ---------------------------------------------------------------------
class Fleet {
public:
    void build(const std::string layout[], int rows);   // PA2.cpp  TODO 5
    void update(float dt);                              // PA2.cpp  TODO 6a (the turn is provided)
    void removeDead();                                  // PA2.cpp  provided (read it)
    float lowestY() const;                              // PA2.cpp  TODO 6b

    int count() const { return static_cast<int>(enemies_.size()); }   // provided
    float direction() const { return direction_; }                    // provided
    float speed() const { return speed_; }                            // provided
    void setSpeed(float speed) { speed_ = speed; }                    // provided

    // provided: read access (for drawing) and write access (for PA3)
    const std::vector<std::unique_ptr<Enemy>>& enemies() const { return enemies_; }
    std::vector<std::unique_ptr<Enemy>>& enemies() { return enemies_; }

private:
    std::vector<std::unique_ptr<Enemy>> enemies_;   // the fleet owns its enemies
    float direction_ = 1.0f;                        // +1 = moving right, -1 = moving left
    float speed_ = FLEET_SPEED;                     // pixels per second
};
