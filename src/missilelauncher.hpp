#pragma once
#include "game.hpp"
#include "missile.hpp"
#include "tower.hpp"

#include <cmath>

class MissileLauncher : public Tower {
   // TODO: Make protected.
	//protected:
	public:
    MissileLauncher(int radius, int fireRate, int price, Game &game,
		    Pos position)
	: Tower(radius, fireRate, price, game, position, 250){};

   public:
    ~MissileLauncher() = default;

    void fire();
    void onDestroy();
    void update();
    int sell();
};

class BasicMissileLauncher : public MissileLauncher {
   public:
	BasicMissileLauncher(Game &game, Pos position) : MissileLauncher(64 * 10, 3, 8000, game, position) {}
};

