
#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{
  if(alien != nullptr)
  {
    delete [] alien;
    alien = nullptr;
  }

  if(shots != nullptr)
  {
    delete [] shots;
    shots = nullptr;
  }
}

bool Game::init()
{
  //init menu background
  background.setSize(sf::Vector2f(1920,720));
  background.setFillColor(sf::Color(255,255,255));

  //init game background
  background_game.setSize(sf::Vector2f(1920,720));
  background_game.setFillColor(sf::Color(0,0,0));

  //init font
  if(!font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout << "font did not load \n";
  }

  //init menu
  in_menu = true;
  menuText();

  //init win message
  win_text.setString("Congratulations, you win!\n    Press escape to exit.");
  win_text.setFont(font);
  win_text.setCharacterSize(50);
  win_text.setFillColor(sf::Color(255,255,255));

  win_text.setPosition(window.getSize().x/2 - win_text.getGlobalBounds().width/2 , window.getSize().y/2 - win_text.getGlobalBounds().height/2);

  //init lose message
  lose_text.setString("         You lose!\nPress escape to exit.");
  lose_text.setFont(font);
  lose_text.setCharacterSize(50);
  lose_text.setFillColor(sf::Color(255,255,255));

  lose_text.setPosition(window.getSize().x/2 - lose_text.getGlobalBounds().width/2 , window.getSize().y/2 - lose_text.getGlobalBounds().height/2);

  //init score
  score_text.setFont(font);
  score_text.setCharacterSize(50);
  score_text.setFillColor(sf::Color(255,255,255,255));

  score_text.setPosition(
    window.getSize().x/2 - score_text.getGlobalBounds().width/2 + 300,
    window.getSize().y/2 - score_text.getGlobalBounds().height/2 + 250);

  //init ship sprite
  ship.initialiseSprite(ship_texture,"Data/Images/SpaceShooterRedux/PNG/playerShip3_red.png");
  ship.getSprite()->setScale(0.7,0.7);
  ship.getSprite()->setPosition(window.getSize().x/2 - ship.getSprite()->getGlobalBounds().width/2, window.getSize().y/2 - ship.getSprite()->getGlobalBounds().height/2 + 250);
  ship.speed = 60;

  //init alien array
  alien = new GameObject [grid_x * grid_y];

  for(int i = 0; i < grid_x; i++)
  {
    for(int j = 0; j < grid_y; j++)
    {
      //Green Aliens
      if((i * grid_y + j) % 5 == 0 || (i * grid_y + j) % 5 == 2 || (i * grid_y + j) % 5 == 4)
      {
        alien[i* grid_y+ j].initialiseSprite(green_alien_texture,"Data/Images/Alien UFO pack/PNG/shipGreen_manned.png");
      }

      //Blue Aliens
      else
      {
        alien[i* grid_y+ j].initialiseSprite(blue_alien_texture,"Data/Images/Alien UFO pack/PNG/shipBlue_manned.png");
      }
      alien[i* grid_y+ j].getSprite()->setScale(0.4,0.4);
      alien[i* grid_y+ j].getSprite()->setPosition(80 * i + 120,50 + 60 * j);
      alien[i* grid_y+ j].direction.x = -0.1;
      alien[i* grid_y+ j].direction.y = 0.01;
      alien[i* grid_y+ j].setVisibility(true);
    }
  }

  //init laser array
  shots = new GameObject [number_of_shots];

  for(int i = 0; i < number_of_shots; i++)
  {
    shots[i].initialiseSprite(shots_texture,"Data/Images/SpaceShooterRedux/PNG/Lasers/laserGreen08.png");
    shots[i].setVisibility(false);
    shots[i].getSprite()->setScale(1,1);
    shots[i].getSprite()->setPosition(ship.getSprite()->getPosition().x + 25,ship.getSprite()->getPosition().y);
    shots[i].direction.y = -35;
    shots[i].speed = 10;
  }

  return true;
}

void Game::alienMove(float dt)
{
  //init counter
  sf::Time counter = clock.getElapsedTime();
  int time_elapsed = int(counter.asSeconds());

  alienBounce();

  //STRAIGHT LINE
  if(menu_index == 0)
  {
    for(int i = 0; i < grid_x * grid_y; i++)
    {
      alien[i].getSprite()->move(alien[i].direction.x * alien->speed * dt, alien[i].direction.y * alien->speed * dt);
    }
  }

  //GRAVITY
  else if(menu_index == 1)
  {
    clock.restart();

    for (int i = 0; i < grid_x * grid_y; i++)
    {
      alien[i].direction.y = 0.00001;
      alien[i].getSprite()->move(alien[i].direction.x * alien->speed * dt, alien[i].direction.y + gravity * alien->speed * dt );

      //Increase gravity every 3 secs;
      if(time_elapsed % 3 == 0)
      {
        gravity += 0.000001;
      }
    }
  }

  //QUADRATIC
  else if(menu_index == 2)
  {
    for (int i = 0; i < grid_x * grid_y; i++)
    {
      alien[i].direction.y = -0.0000001 * (window.getSize().x/2 - alien[i].getSprite()->getPosition().x) * (window.getSize().x/2 - alien[i].getSprite()->getPosition().x) + 0.01;

      alien[i].getSprite()->move(alien[i].direction.x * alien->speed * dt, alien[i].direction.y * alien->speed * dt + 0.025);
    }
  }

  //CURVE
  else if(menu_index == 3)
  {
    for (int i = 0; i < grid_x * grid_y; i++)
    {
      alien[i].direction.y = -0.015 * std::sin (0.015 * (window.getSize().x/2 - alien[i].getSprite()->getPosition().x + 10));

      alien[i].getSprite()->move(alien[i].direction.x * alien->speed * dt, alien[i].direction.y * alien->speed * dt + 0.025);
    }
  }
}

void Game::alienBounce()
{
  //Alien Column Check
  for(int i = 0; i < grid_x * grid_y; i++)
  {
    if (!alien[alien_row_left].getVisibility())
    {
      alien_row_left++;
    }

    if (!alien[alien_row_right].getVisibility())
    {
      alien_row_right--;
    }

    //Left bounce
    if(alien[alien_row_left].getSprite()->getPosition().x <= 0)
    {
      if(alien[i].direction.x <= 0)
      {
        alien[i].direction.x = -alien[i].direction.x;
        alien[i].getSprite()->setPosition(alien[i].getSprite()->getPosition().x,alien[i].getSprite()->getPosition().y + 10);
      }
    }

    //Right bounce
    if(alien[alien_row_right].getSprite()->getPosition().x >= (window.getSize().x - alien[alien_row_right].getSprite()->getGlobalBounds().width))
    {
      if(alien[i].direction.x >= 0)
      {
        alien[i].direction.x = -alien[i].direction.x;
        alien[i].getSprite()->setPosition(alien[i].getSprite()->getPosition().x,alien[i].getSprite()->getPosition().y + 10);
      }
    }
  }
}


void Game::shoot(float dt)
{
  for(int i = 0; i < number_of_shots ; i++)
  {
    if(shots[i].getVisibility())
    {
      //Shot movement
      shots[i].getSprite()->move(0,shots[i].direction.y * shots->speed * dt);

      for(int j = 0; j < grid_x * grid_y; j++)
      {
        if(shots[i].getVisibility() && alien[j].getVisibility())
        {
          //Shot collision with alien
          if(shots[i].getSprite()->getGlobalBounds().intersects(alien[j].getSprite()->getGlobalBounds()))
          {
            alien[j].setVisibility(false);
            shots[i].setVisibility(false);

            //Increase speed for straight line
            if(menu_index == 0)
            {
              alien->speed +=30;
            }
            score++;
          }
        }
      }
      //Shots hit the top
      if(shots[i].getSprite()->getPosition().y <= 0)
      {
        if(current_shot_number > 0)
        {
          current_shot_number--;
        }
        shots[i].setVisibility(false);
      }
    }
  }
}

void Game::hit()
{
  //Ship collision with alien and bottom of window.
  for(int i = 0; i < grid_x * grid_y; i++)
  {
    if(alien[i].getVisibility())
    {
      if(ship.getSprite()->getGlobalBounds().intersects(alien[i].getSprite()->getGlobalBounds()) || alien[i].getSprite()->getPosition().y + alien[i].getSprite()->getLocalBounds().height >= window.getSize().y )
      {
        is_hit = true;
      }
    }
  }
}


void Game::update(float dt)
{
  if(!in_menu)
  {
    //Clamp player boundaries
    if(0 < ship.getSprite()->getPosition().x  && ship.getSprite()->getPosition().x < window.getSize().x - ship.getSprite()->getGlobalBounds().width)
    {
      ship.getSprite()->move(ship.direction.x * ship.speed * dt,0);
    }
    else if(0 > ship.getSprite()->getPosition().x)
    {
      ship.getSprite()->setPosition(ship.getSprite()->getPosition().x + 1, ship.getSprite()->getPosition().y);
    }
    else if(ship.getSprite()->getPosition().x > window.getSize().x - ship.getSprite()->getGlobalBounds().width)
    {
      ship.getSprite()->setPosition(ship.getSprite()->getPosition().x - 1, ship.getSprite()->getPosition().y);
    }

    alienMove(dt);
    shoot(dt);
    hit();

    //Update score
    std::string new_score = std::to_string(score);
    score_text.setString("Score: " + new_score);
  }
}

void Game::render()
{
  if(in_menu)
  {
    window.draw(background);
    window.draw(play_text);
    window.draw(select_text);

    for(int i = 0; i < 5 ; i++)
    {
      window.draw(menu[i]);
    }
  }
  else
  {
    window.draw(background_game);

    for(int i = 0; i < grid_x * grid_y ; i++)
    {
      if(alien[i].getVisibility())
      {
        window.draw(*alien[i].getSprite());
      }
    }

    for(int i = 0; i < number_of_shots ; i++)
    {
      if(shots[i].getVisibility())
      {
        window.draw(*shots[i].getSprite());
      }
    }
    window.draw(*ship.getSprite());
    window.draw(score_text);

    if(score >= grid_x * grid_y)
    {
      window.draw(background_game);
      window.draw(win_text);
    }

    if(is_hit)
    {
      window.draw(background_game);
      window.draw(lose_text);
    }
  }
}

void Game::keyPressed(sf::Event event)
{
  if(event.key.code == sf::Keyboard::Enter)
  {
    if (in_menu)
    {
      in_menu = false;
    }
  }

  if(event.key.code == sf::Keyboard::Space)
  {
    current_shot_number ++;

    //Show shots
    if(current_shot_number < number_of_shots)
    {
      shots[current_shot_number].setVisibility(true);

      //Shot position
      if(shots[current_shot_number].getVisibility())
      {
        shots[current_shot_number].getSprite()->setPosition(ship.getSprite()->getPosition().x + 28, ship.getSprite()->getPosition().y);
      }
    }
    else
    {
      //Reset shot
      shots[current_shot_number].setVisibility(false);
      current_shot_number = -1;
    }
  }

  if(event.key.code == sf::Keyboard::A)
  {
    if(ship.getSprite()->getPosition().x <= 0)
    {
      ship.direction.x = 0;
      ship.speed = 0;
    }
    else
    {
      ship.direction.x = -10;
      ship.speed = 60;
    }
  }

  if(event.key.code == sf::Keyboard::D)
  {
    if(ship.getSprite()->getPosition().x >= (window.getSize().x - ship.getSprite()->getGlobalBounds().width))
    {
      ship.direction.x = 0;
      ship.speed = 0;
    }
    else
    {
      ship.direction.x = 10;
      ship.speed = 60;
    }
  }

  if(event.key.code == sf::Keyboard::Up)
  {
    if(in_menu)
    {
      if(menu_index - 1 >= 0)
      {
        menu[menu_index].setFillColor(sf::Color(sf::Color::Black));
        menu_index--;
        menu[menu_index].setFillColor(sf::Color(sf::Color::Red));
      }
    }
  }

  if(event.key.code == sf::Keyboard::Down)
  {
    if(in_menu)
    {
      if(menu_index + 1 < 4)
      {
        menu[menu_index].setFillColor(sf::Color(sf::Color::Black));
        menu_index++;
        menu[menu_index].setFillColor(sf::Color(sf::Color::Red));
      }
    }
  }

  if(event.key.code == sf::Keyboard::Escape)
  {
    if ((is_hit) || (score >= grid_x * grid_y ))
    {
      window.close();
    }
  }
}
void Game::keyReleased(sf::Event event)
{
  if(event.key.code == sf::Keyboard::A)
  {
    ship.direction.x = 0;
  }

  if(event.key.code == sf::Keyboard::D)
  {
    ship.direction.x = 0;
  }
}
void Game::menuText()
{
  //init title message
  play_text.setString("SPACE INVADERS!");
  play_text.setFont(font);
  play_text.setCharacterSize(70);
  play_text.setFillColor(sf::Color(0,0,0));

  play_text.setPosition(
    window.getSize().x/2 - play_text.getGlobalBounds().width/2,
    window.getSize().y/2 - play_text.getGlobalBounds().height/2 - 250);

  //init instructional message
  select_text.setString("Use the arrow keys and""\n Enter to select mode");
  select_text.setFont(font);
  select_text.setCharacterSize(30);
  select_text.setFillColor(sf::Color(0,0,0));

  select_text.setPosition(
    window.getSize().x/2 - select_text.getGlobalBounds().width/2 + 250,
    window.getSize().y/2 - select_text.getGlobalBounds().height/2);

  //init menu
  menu[0].setString("Straight line");
  menu[0].setFont(font);
  menu[0].setCharacterSize(30);
  menu[0].setFillColor(sf::Color(255,0,0));

  menu[0].setPosition(
    window.getSize().x/2 - play_text.getGlobalBounds().width/2,
    window.getSize().y/2 - play_text.getGlobalBounds().height/2 - 70);

  menu[1].setString("Gravity");
  menu[1].setFont(font);
  menu[1].setCharacterSize(30);
  menu[1].setFillColor(sf::Color(0,0,0));

  menu[1].setPosition(
    window.getSize().x/2 - play_text.getGlobalBounds().width/2,
    window.getSize().y/2 - play_text.getGlobalBounds().height/2 - 10);

  menu[2].setString("Quadratic");
  menu[2].setFont(font);
  menu[2].setCharacterSize(30);
  menu[2].setFillColor(sf::Color(0,0,0));

  menu[2].setPosition(
    window.getSize().x/2 - play_text.getGlobalBounds().width/2,
    window.getSize().y/2 - play_text.getGlobalBounds().height/2 + 50);

  menu[3].setString("Curve");
  menu[3].setFont(font);
  menu[3].setCharacterSize(30);
  menu[3].setFillColor(sf::Color(0,0,0));

  menu[3].setPosition(
    window.getSize().x/2 - play_text.getGlobalBounds().width/2,
    window.getSize().y/2 - play_text.getGlobalBounds().height/2 + 110);
}
