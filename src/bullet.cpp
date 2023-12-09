#include "bullet.hpp"
#include "utility_func_game.hpp"
#include "gamegrid.hpp"

void Bullet::update() {
    // Find the vector between starting point and end point.
    Pos vec = Pos{ (endPos_.x - startPos_.x), (endPos_.y - startPos_.y) };
    float vecLength = sqrt((vec.x * vec.x) + (vec.y * vec.y));

    // Find unit vector of vec.
    Pos unitVec = Pos{ (vec.x / vecLength), (vec.y / vecLength) };

    // Check, whether the enemy is within bullet radius on every position update.
    int i = 0;

    while (i < this->travel_speed()) {
        // Hit the first enemy on the Bullet's path, if there is one.
        std::vector<Enemy*> enemies = this->getEnemiesWithinRadius();

        if (!enemies.empty()) {
            enemies.at(0)->takeDamage(this->damage());
            health_ = 0;
            break;
        }

        // Update bullet's position.
        position_.x += unitVec.x;
        position_.y += unitVec.y;

        // Check that the Bullet is still on the screen.
        bool inX = ((0 <= this->position_.x) && (this->position_.x < (getGrid(game_)->Width() * getGrid(game_)->TileWidth() - 1)));
        bool inY = ((0 <= this->position_.y) && (this->position_.y < (getGrid(game_)->Height() * getGrid(game_)->TileWidth() - 1)));

        if (!(inX && inY)) {
            health_ = 0;
            break;
        }

        i++;
    }
}

void Bullet::onDestroy() {
}
