#pragma once
#include "enemy.hpp"

// TODO: Implement methods
class FootSoldier: public Enemy {
private:

public:
    FootSoldier(int speed, int value, int price, int hp) : Enemy(speed, value, price, hp) {};

    void update();
    void onDestroy();
};
