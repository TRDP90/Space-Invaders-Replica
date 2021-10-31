
#ifndef SPACEINVADERS_GAMEOBJECT_H
#define SPACEINVADERS_GAMEOBJECT_H
#include <SFML/Graphics.hpp>
#include "Vector2.h"


class GameObject

{
 public:
  GameObject();
  ~GameObject();
  bool initialiseSprite(sf::Texture &texture, std::string filename);
  sf::Sprite* getSprite();

  Vector2 direction = {0,0};
  float speed = 200;
  bool setVisibility(bool value);
  bool getVisibility();

 private:
  sf::Sprite* sprite = nullptr;
  bool visibility = true;

};





#endif // SPACEINVADERS_GAMEOBJECT_H
