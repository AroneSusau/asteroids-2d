#ifndef H_PHYSICS
#define H_PHYSICS

#include "./../standard.h"
#include "./Spaceship.h"
#include "./Wall.h"
#include "./Util.h"
#include "./Asteroid.h"
#include "./Bullet.h"
#include "./../settings/game_settings.h"

class Physics_t {

  public:
    float time = 0;
    float cur_time = 0;

    void move_ship(spaceship_t &spaceship);
    void move_asteroids(std::vector<Asteroid> &asteroids, arena_t &arena);
    void move_bullet(std::vector<Bullet> &bullets, arena_t &arena);

    void ship_wall_warning(spaceship_t &spaceship, Wall walls [4], arena_t arena);

    void ship_wall_collision(spaceship_t &spaceship, arena_t &arena);
    void asteroids_wall_collision(std::vector<Asteroid> &asteroids, arena_t &arena);
    void ship_asteroid_collision(std::vector<Asteroid> &asteroids, spaceship_t &spaceship, arena_t &arena);
    void bullet_asteroid_collision(std::vector<Bullet> &bullets, std::vector<Asteroid> &asteroids, std::vector<ParticleGenerator> &particles_generators, arena_t &arena);

};

#endif