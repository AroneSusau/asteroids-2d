#ifndef H_PHYSICS
#define H_PHYSICS

#include "./../standard.h"
#include "./Spaceship.h"
#include "./Wall.h"
#include "./Util.h"
#include "./../settings/game_settings.h"

class Physics_t {

  public:
    void move_ship(spaceship_t &spaceship);

    void ship_wall_warning(spaceship_t &spaceship, Wall walls [4], arena_t arena);

};

#endif