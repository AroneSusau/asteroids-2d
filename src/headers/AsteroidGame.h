#ifndef H_ASTEROID_GAME
#define H_ASTEROID_GAME

#include "standard.h"
#include "settings/game_settings.h"
#include "constants/keyboard.h"
#include "classes/GraphicsRenderer.h"
#include "classes/Spaceship.h"
#include "classes/Physics.h"
#include "classes/Asteroid.h"
#include "classes/ParticleGenerator.h"
#include "classes/Bullet.h"

class AsteroidGame {
  
  public:
    AsteroidGame();
    ~AsteroidGame();

    static void run(int argc, char** argv);
    static void init(int argc, char** argv);
    static void init_game_objs();

    static void update_particle_generators(float dt);

    static void on_key_press(unsigned char key, int x, int y);
    static void on_key_release(unsigned char key, int x, int y);
    static void on_reshape(int w, int h);

    static void on_idle();

    static void spawn_asteroids(float dt);
    static void spawn_bullets(float dt);
    static void generate_asteroid();

    static void display();
    static void on_mouse_event(int button, int state, int x, int y);
};

#endif
