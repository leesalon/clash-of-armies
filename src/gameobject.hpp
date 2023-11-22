#pragma once

#include <SFML/Graphics.hpp>

#include "pos.hpp"

class GameObject {
 public:
  GameObject(sf::Sprite sprite, int health = 0)
      : sprite_(sprite), health_(health){};

  sf::Sprite GetSprite() const;

 protected:
  sf::Sprite sprite_;
  int health_;
  Pos position_;
};
