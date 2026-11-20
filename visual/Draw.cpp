// =====================================================================
// Draw.cpp -- draws the game objects                    (PROVIDED, visual)
//
// Pictures are "pixel art": a small grid of '1' (draw) and '0' (empty),
// stretched to fill an object's bounds() rectangle.
// You do not need to change this file for PA1-PA4.
// =====================================================================
#include "Draw.hpp"
#include "PixelFont.hpp"

namespace {   // helpers private to this file

// ---------------- pixel-art pictures ----------------------------------
// Each picture is a list of rows; '1' = colored pixel.
const int ALIEN_ROWS = 8;
const char* NORMAL_ALIEN[ALIEN_ROWS] = {
    "00111100",
    "01111110",
    "11011011",
    "11111111",
    "10111101",
    "10100101",
    "00100100",
    "01000010",
};
const char* RED_ALIEN[ALIEN_ROWS] = {
    "00100100",
    "00011000",
    "01111110",
    "11011011",
    "11111111",
    "00100100",
    "01011010",
    "10100101",
};
const char* ARMORED_ALIEN[ALIEN_ROWS] = {
    "01111110",
    "11111111",
    "11011011",
    "11111111",
    "11111111",
    "01100110",
    "11000011",
    "10000001",
};
// EXTENSION: a picture for your own enemy type (used for any unknown type()).
const char* UNKNOWN_ALIEN[ALIEN_ROWS] = {
    "11111111",
    "10000001",
    "10111101",
    "10100101",
    "10100101",
    "10111101",
    "10000001",
    "11111111",
};

const int SHIP_ROWS = 7;
const char* SHIP[SHIP_ROWS] = {
    "000011110000",
    "000111111000",
    "000111111000",
    "111111111111",
    "111111111111",
    "111111111111",
    "110011110011",
};

// Draw a picture (rows of '1'/'0') stretched over the rectangle area.
void drawPicture(sf::RenderTarget& window, const char* const rows[], int rowCount,
                 int colCount, Rect area, sf::Color color) {
    float pixelW = area.w / colCount;
    float pixelH = area.h / rowCount;
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            if (rows[r][c] == '1') {
                // +0.2f closes hairline gaps between neighbouring pixels
                drawBox(window, area.x + c * pixelW, area.y + r * pixelH,
                        pixelW + 0.2f, pixelH + 0.2f, color);
            }
        }
    }
}

}  // namespace

void drawBackground(sf::RenderTarget& window) {
    window.clear(COLOR_BACKGROUND);
    // 100 stars at fixed, scattered places (the formulas just spread them out)
    for (int i = 0; i < 100; i++) {
        float x = static_cast<float>((i * 193 + 57) % 795);
        float y = static_cast<float>((i * 127 + 81) % 595);
        float size = (i % 5 == 0) ? 2.0f : 1.0f;
        drawBox(window, x, y, size, size, COLOR_STAR);
    }
    drawBox(window, 0.0f, 49.0f, WIDTH, 2.0f, COLOR_LINE);    // under the top bar
    drawBox(window, 0.0f, 565.0f, WIDTH, 2.0f, COLOR_LINE);   // above the bottom bar
}

void drawPlayer(sf::RenderTarget& window, const Player& player, bool visible) {
    if (!visible) return;
    drawPicture(window, SHIP, SHIP_ROWS, 12, player.bounds(), COLOR_CYAN);
}

void drawShots(sf::RenderTarget& window, const Weapon& weapon) {
    for (const Projectile& shot : weapon.shots()) {
        if (!shot.alive()) continue;   // dead shots are not drawn
        Rect b = shot.bounds();
        drawBox(window, b.x, b.y, b.w, b.h, shot.friendly() ? COLOR_CYAN : COLOR_GOLD);
    }
}

void drawFleet(sf::RenderTarget& window, const Fleet& fleet) {
    for (const std::unique_ptr<Enemy>& enemy : fleet.enemies()) {
        if (!enemy->alive()) continue;
        Rect area = enemy->bounds();
        // type() is virtual: through an Enemy pointer we still get the
        // answer of the real object ('R', 'A' or 'N') -- polymorphism.
        char type = enemy->type();
        if (type == 'R') {
            drawPicture(window, RED_ALIEN, ALIEN_ROWS, 8, area, COLOR_RED);
        } else if (type == 'A') {
            // An armored enemy that has already been hit is drawn darker.
            // dynamic_cast asks "is this Enemy really an ArmoredEnemy?";
            // it gives nullptr if not. (Not needed for the PAs.)
            sf::Color color = COLOR_GOLD;
            const ArmoredEnemy* armored = dynamic_cast<const ArmoredEnemy*>(enemy.get());
            if (armored != nullptr && armored->armor() < ARMOR_HITS) color = sf::Color(150, 110, 50);
            drawPicture(window, ARMORED_ALIEN, ALIEN_ROWS, 8, area, color);
        } else if (type == 'N') {
            drawPicture(window, NORMAL_ALIEN, ALIEN_ROWS, 8, area, COLOR_PURPLE);
        } else {
            // EXTENSION: give your new enemy type its own picture/color here.
            drawPicture(window, UNKNOWN_ALIEN, ALIEN_ROWS, 8, area, sf::Color::White);
        }
    }
}

void drawHud(sf::RenderTarget& window, int score, int lives, int best,
             const std::string& paStatus) {
    drawText(window, "SPACE INVADERS", 22.0f, 14.0f, 3.0f, COLOR_CYAN);
    drawText(window, "SCORE", 560.0f, 12.0f, 2.0f, COLOR_PALE);
    drawText(window, std::to_string(score), 630.0f, 12.0f, 2.0f, COLOR_GOLD);
    drawText(window, "BEST", 560.0f, 30.0f, 2.0f, COLOR_PALE);
    drawText(window, std::to_string(best), 630.0f, 30.0f, 2.0f, COLOR_GOLD);

    drawText(window, "LIVES", 20.0f, 576.0f, 2.0f, COLOR_PALE);
    for (int i = 0; i < lives && i < 10; i++) {   // one small ship per life
        drawPicture(window, SHIP, SHIP_ROWS, 12, {90.0f + i * 30.0f, 576.0f, 24.0f, 14.0f}, COLOR_CYAN);
    }
    drawText(window, paStatus, WIDTH - 20.0f - textWidth(paStatus, 2.0f), 576.0f, 2.0f, COLOR_PALE);
}

void drawMessage(sf::RenderTarget& window, const std::string& big,
                 sf::Color bigColor, const std::string& small) {
    drawBox(window, 115.0f, 221.0f, 570.0f, 120.0f, COLOR_PANEL);
    drawBox(window, 115.0f, 221.0f, 570.0f, 2.0f, COLOR_CYAN);
    drawBox(window, 115.0f, 339.0f, 570.0f, 2.0f, COLOR_CYAN);
    drawText(window, big, (WIDTH - textWidth(big, 4.0f)) / 2.0f, 246.0f, 4.0f, bigColor);
    drawText(window, small, (WIDTH - textWidth(small, 2.0f)) / 2.0f, 307.0f, 2.0f, COLOR_PALE);
}
