#pragma once
#include <cmath>

#include "projectile.hpp"
#include "enemy.hpp"

// TODO: Implement methods
class Missile: public Projectile {
private:
    Enemy* target_;
    double explosionRadius_;
    
public:
    Missile(sf::Sprite sprite, Game& game, int travel_speed, int damage, int radius, Enemy* target, Pos position, double exp_radius = 0)
        : Projectile(sprite, game, travel_speed, damage, radius, position), target_(target), explosionRadius_(exp_radius) {};
    // Vec2D speed_vec;
    void update();
    void onDestroy() {};
};
