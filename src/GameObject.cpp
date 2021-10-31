#include "GameObject.h"
#include "iostream"

bool GameObject::initialiseSprite(sf::Texture& texture, std::string filename)
{
  //init Sprite
  if(!texture.loadFromFile(filename))
  {
    std::cout << "Texture did not load\n";
  }

  sprite->setTexture(texture);
  sprite->setPosition(
    64 - sprite->getGlobalBounds().width/2,
    64 - sprite->getGlobalBounds().height/2);
  sprite -> setScale(0.3,0.3);

  return true;
}

GameObject::GameObject()
{
  sprite = new sf::Sprite;
}


GameObject::~GameObject()
{
}


sf::Sprite* GameObject::getSprite()
{
  return sprite;
}


bool GameObject::getVisibility()
{
  return visibility;
}


bool GameObject::setVisibility(bool value)
{
  visibility = value;
}