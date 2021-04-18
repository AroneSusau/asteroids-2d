#ifndef H_PHYSICS
#define H_PHYSICS

#include "./../standard.h"
#include "./Spaceship.h"
#include "./Wall.h"
#include "./Util.h"
#include "./Asteroid.h"
#include "./Bullet.h"
#include "./Blackhole.h"
#include "./../settings/game_settings.h"

class Physics_t {

  public:
    float time = 0;
    float cur_time = 0;

    void move_ship(spaceship_t &spaceship);
    void move_asteroids(std::vector<Asteroid> &asteroids, arena_t &arena);
    void move_bullet(std::vector<Bullet> &bullets, arena_t &arena);

    void move_black_hole_rings(BlackHole &hole, float dt);

    void ship_wall_warning(spaceship_t &spaceship, Wall walls [4], arena_t arena);

    void ship_wall_collision(spaceship_t &spaceship, arena_t &arena);
    void asteroids_wall_collision(std::vector<Asteroid> &asteroids, arena_t &arena);
    void ship_asteroid_collision(std::vector<Asteroid> &asteroids, spaceship_t &spaceship, arena_t &arena);
    void bullet_asteroid_collision(std::vector<Bullet> &bullets, std::vector<Asteroid> &asteroids, std::vector<ParticleGenerator> &particles_generators, arena_t &arena);
    void asteroid_to_asteroid_collision(std::vector<Asteroid> &asteroids);

    void blackhole_to_asteroid_collision(BlackHole hole, std::vector<Asteroid> &asteroids, std::vector<ParticleGenerator> &particles_generators);
    void blackhole_to_ship_collision(BlackHole hole, spaceship_t &spaceship, arena_t &arena);
    void blackhole_pull_effect(BlackHole hole, spaceship_t &spaceship, std::vector<Asteroid> &asteroids);
    
    float elastic_collsion(float dv1, float dv2, float m1, float m2);
    bool circlular_collision(float x1, float y1, float r1, float x2, float y2, float r2);

};

#endif