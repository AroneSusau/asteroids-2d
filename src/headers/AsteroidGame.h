#ifndef H_ASTEROID_GAME
#define H_ASTEROID_GAME

#include "../headers/standard.h"

#include "../headers/settings/game_settings.h"
#include "../headers/constants/keyboard.h"

class AsteroidGame {
private:
  /* data */
public:
  AsteroidGame(/* args */);
  ~AsteroidGame();

  static void run(int argc, char** argv);
  static void init(int argc, char** argv);

  static void on_key_press(unsigned char key, int x, int y);
  static void on_reshape(int w, int h);
  
  static void display();
};

#endif
