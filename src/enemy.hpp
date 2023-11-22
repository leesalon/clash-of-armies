#pragma once
#include "gameobject.hpp"


class Enemy: public GameObject {
protected:
    int speed_;
    int value_;
    int price_;
    int maxHP_;
public:
    explicit Enemy(int speed, int value, int price, int maxHP, sf::Sprite sprite, Game& game)
            : GameObject(sprite, game), speed_(speed), value_(value), price_(price), maxHP_(maxHP) {};
    
    virtual ~Enemy() {};

    bool isAlive() const;
};
