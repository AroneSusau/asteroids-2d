#ifndef H_PHYSICS
#define H_PHYSICS

#include "./../standard.h"
#include "./Spaceship.h"
#include "./Wall.h"
#include "./Util.h"
#include "./../settings/game_settings.h"

class Physics_t {

  public:
    float time = 0;
    float cur_time = 0;

    void move_ship(spaceship_t &spaceship);

    void ship_wall_warning(spaceship_t &spaceship, Wall walls [4], arena_t arena);

    void ship_wall_collision(spaceship_t &spaceship, Wall walls [4], arena_t arena);

};

#endif