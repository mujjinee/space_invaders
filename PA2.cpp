// =====================================================================
// PA2.cpp -- Enemies and the Fleet                       (YOU WRITE THIS)
//
// Read PA2.hpp first: it lists every number you need as a named constant.
//
// Work TODO by TODO (6 TODOs). After each TODO, run its test group:
//     TODO 1-2 -> group 1    TODO 3-4 -> group 2
//     TODO 5   -> group 3    TODO 6   -> group 4
// The turn at the wall (in Fleet::update) and Fleet::removeDead are
// provided: read them, do not change them.
// Do not add main(), printing, or SFML code to this file.
// =====================================================================
#include "PA2.hpp"

// ===================== TODO 1: Enemy basics ===========================
// Constructor: store x and y in x_ and y_. A new enemy is alive.
Enemy::Enemy(float x, float y) : x_(x), y_(y), alive_(true) {}

// translate: move the enemy by dx (right is +) and dy (down is +).
void Enemy::translate(float dx, float dy) {
    x_ += dx;
    y_ += dy;
}

// bounds: top-left (x_, y_), size ENEMY_W x ENEMY_H.
Rect Enemy::bounds() const {
    return {x_, y_, ENEMY_W, ENEMY_H};
}

// ===================== TODO 2: Enemy virtual functions ================
// A normal enemy: worth NORMAL_POINTS, type 'N', and one hit destroys it
// (alive_ becomes false).
int Enemy::points() const {
    return NORMAL_POINTS;
}

char Enemy::type() const {
    return 'N';
}

void Enemy::hit() {
    alive_ = false;
}

// ===================== TODO 3: RedEnemy ===============================
// RedEnemy "is an" Enemy. Its constructor only passes x and y on to the
// Enemy constructor -- that is what ": Enemy(x, y)" below does. It reuses
// x(), y(), translate(), bounds() and hit() from Enemy: do not copy them.
RedEnemy::RedEnemy(float x, float y) : Enemy(x, y) {}

// Override the two virtual functions: RED_POINTS and type 'R'.
int RedEnemy::points() const {
    return RED_POINTS;
}

char RedEnemy::type() const {
    return 'R';
}

// ===================== TODO 4: ArmoredEnemy ===========================
// Constructor: ": Enemy(x, y)" (given) passes x and y on to Enemy, as in
// RedEnemy. Inside the body, set armor_ to ARMOR_HITS.
ArmoredEnemy::ArmoredEnemy(float x, float y) : Enemy(x, y) {
    armor_ = ARMOR_HITS;
}

// ARMORED_POINTS and type 'A'.
int ArmoredEnemy::points() const {
    return ARMORED_POINTS;
}

char ArmoredEnemy::type() const {
    return 'A';
}

// hit: one hit lowers armor_ by 1. When armor_ reaches 0, the enemy is
// destroyed. alive_ is PRIVATE in Enemy, so you cannot set it here --
// call the base-class version instead:   Enemy::hit();
void ArmoredEnemy::hit() {
    if (armor_ > 0) --armor_;
    if (armor_ == 0) Enemy::hit();
}

// ===================== TODO 5: Fleet::build ===========================
// Create one enemy for each letter of the layout (see FORMATION in PA2.hpp).
//   layout[row]       is a string: one row of the formation
//   layout[row][col]  is a char:   'R', 'A', 'N' or '.'
//
// The enemy in row r, column c goes to
//     x = FLEET_LEFT + c * COLUMN_GAP,     y = FLEET_TOP + r * ROW_GAP
//
// Each enemy is owned by a unique_ptr<Enemy>, even a RedEnemy or an
// ArmoredEnemy (a base-class pointer can point to a derived object):
//     enemies_.push_back(std::unique_ptr<Enemy>(new RedEnemy(x, y)));
// You never write delete: the unique_ptr deletes the enemy when the vector
// removes it.
void Fleet::build(const std::string layout[], int rows) {
    enemies_.clear();      // provided: forget the old enemies (they are deleted)
    direction_ = 1.0f;     // provided: a new fleet starts moving right

    for (int row = 0; row < rows; ++row) {
        for (std::size_t col = 0; col < layout[row].size(); ++col) {
            const float x = FLEET_LEFT + col * COLUMN_GAP;
            const float y = FLEET_TOP + row * ROW_GAP;
            switch (layout[row][col]) {
                case 'R':
                    enemies_.push_back(std::make_unique<RedEnemy>(x, y));
                    break;
                case 'A':
                    enemies_.push_back(std::make_unique<ArmoredEnemy>(x, y));
                    break;
                case 'N':
                    enemies_.push_back(std::make_unique<Enemy>(x, y));
                    break;
                default:
                    break;
            }
        }
    }
}

// ===================== TODO 6: Fleet::update and lowestY ==============
// The whole fleet moves sideways together. When it touches a wall, it
// turns round and steps down -- like the classic arcade game.
// Steps 1 and 2 (the turn) are PROVIDED: read them. You write step 3.
void Fleet::update(float dt) {
    if (enemies_.empty() || dt <= 0.0f) return;

    // 1. PROVIDED: find the fleet's left edge and right edge
    //    (the minimum and maximum, as with an array).
    float left = enemies_[0]->x();
    float right = enemies_[0]->x() + ENEMY_W;
    for (const std::unique_ptr<Enemy>& enemy : enemies_) {
        if (enemy->x() < left) left = enemy->x();
        if (enemy->x() + ENEMY_W > right) right = enemy->x() + ENEMY_W;
    }

    // 2. PROVIDED: at a wall, turn round and move EVERY enemy down.
    bool atRightWall = direction_ > 0.0f && right >= WALL_RIGHT;
    bool atLeftWall = direction_ < 0.0f && left <= WALL_LEFT;
    if (atRightWall || atLeftWall) {
        direction_ = -direction_;
        for (std::unique_ptr<Enemy>& enemy : enemies_) {
            enemy->translate(0.0f, STEP_DOWN);
        }
    }

    // 3. Move the entire fleet in its current direction.
    const float dx = direction_ * speed_ * dt;
    for (const std::unique_ptr<Enemy>& enemy : enemies_) {
        enemy->translate(dx, 0.0f);
    }
}

// ===================== PROVIDED: Fleet::removeDead (read it) ==========
// Erases every enemy whose alive() is false; the others keep their order.
// Erasing the unique_ptr deletes the enemy automatically (RAII): no delete.
// It walks BACKWARDS: erasing shifts the later elements one place to the
// left, so a forward loop would skip the element right after an erased one.
void Fleet::removeDead() {
    for (int i = (int)enemies_.size() - 1; i >= 0; i--) {
        if (!enemies_[i]->alive()) {
            enemies_.erase(enemies_.begin() + i);
        }
    }
}

// ===================== TODO 6b: lowestY ===============================
// lowestY: the largest bottom edge (y + ENEMY_H) of all enemies,
// or 0 if the fleet is empty. PA4 uses it: the player loses when the
// fleet reaches the ship.
// Like finding the maximum of an array:
//     start with the first enemy's bottom edge and keep the largest y + ENEMY_H.
float Fleet::lowestY() const {
    if (enemies_.empty()) return 0.0f;

    float lowest = enemies_.front()->y() + ENEMY_H;
    for (const std::unique_ptr<Enemy>& enemy : enemies_) {
        const float bottom = enemy->y() + ENEMY_H;
        if (bottom > lowest) lowest = bottom;
    }
    return lowest;
}
