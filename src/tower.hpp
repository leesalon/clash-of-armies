#pragma once

#include <cmath>
#include <vector>
#include <cmath>
#include <iostream>

#include "enemy.hpp"
#include "gameobject.hpp"

// TODO: Implement methods
class Tower: public GameObject {
private:
    int radius_;

protected:
    int fireRate_; // game ticks
    int fireIntervalCounter_; // goes from fireRate to 0. Tells when fire()-method is executed.
    int price_;

public:
    /**
     * @brief Construct a new Tower object.
     * 
     * @param radius the radius of damage caused by projectile (int)
    */

    explicit Tower(int radius, int fireRate, int price, sf::Sprite& sprite, Game& game, Pos position)
            : GameObject(sprite, game, position), radius_(radius), fireRate_(fireRate), fireIntervalCounter_(0), price_(price) {};

    /**
     * @brief Returns all the enemies within the given radius (tower radius).
     * Does not take any parameters.
    */
    /* Comment: Shouldn't this belong to Game / Level class? This doesn't seem to be Tower's responsibility. */
    const std::vector<Enemy*> getEnemiesWithinRadius();
    
   /**
     * @brief Destructor of Tower object.
     */
    virtual ~Tower() = 0;

	virtual void update() = 0;
    virtual void fire() = 0;
    // virtual bool takeDamage() = 0;
    virtual void onDestroy() = 0;
    virtual int sell() = 0;

};
