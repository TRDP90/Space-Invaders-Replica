
#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include "GameObject.h"

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void menuText();
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);

  void alienMove(float dt);
  void alienBounce();
  void shoot(float dt);
  void hit();

 private:
  sf::RenderWindow& window;

  sf::Clock clock;

  sf::Font font;
  sf::Text play_text;
  sf::Text select_text;
  sf::Text menu[4];
  sf::Text score_text;
  sf::Text win_text;
  sf::Text lose_text;

  GameObject ship;
  sf::Texture ship_texture;

  sf::RectangleShape background;
  sf::RectangleShape background_game;

  GameObject* alien = nullptr;
  sf::Texture blue_alien_texture;
  sf::Texture green_alien_texture;

  GameObject* shots = nullptr;
  sf::Texture shots_texture;

  int number_of_shots = 3;
  int current_shot_number = -1;

  int alien_row_left = 0;
  int alien_row_right = 54;

  int grid_x = 11;
  int grid_y = 5;
  int score = 0;
  int menu_index = 0;

  float gravity = 0;


  bool in_menu;
  bool is_hit = false;

};

#endif // SPACEINVADERS_GAME_H
