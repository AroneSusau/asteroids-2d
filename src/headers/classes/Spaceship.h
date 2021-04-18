#ifndef H_SPACESHIP
#define H_SPACESHIP

#include "./ParticleGenerator.h"
#include "./../settings/game_settings.h"

typedef enum { solid, wireframe } ship_fill;

static class spaceship_t {
  public:

    // Position & Movement
    float x;
    float y;
    float dx = 0;
    float dy = 0;
    float velocity_max = 10;
    float rotation_max = 15;
    float acceleration = 6;
    float r_acceleration = 5;

    float rotation = 0;
    float dr = 0;

    float bound_radius = 28.28;
    
    // Pew Pew Stuff
    float fire_rate = 0.25;
    float next_fire = 0;
    float damage = 10;

    // Size
    float size = 20;
    float padding = 8;

    // State
    bool forward = false;
    bool left = false;
    bool right = false;
    bool firing = false;

    // Fill & colour
    float fill_colour [3] = { 0.0, 1.0, 0.0 };
    float outline_colour [3] = { 1.0, 1.0, 1.0 };
    float debug_colour [3] = { 0.0, 0.0, 0.6 };
    ship_fill fill_style = wireframe;

    // Debug
    bool debug_bounds = false;

    ParticleGenerator default_trail() {
      ParticleGenerator ship_flames;
      
      ship_flames.active = true;
      ship_flames.tag = ship_trail;
      ship_flames.duration_type = continuous;
      ship_flames.particle_lifetime = 0.1;
      ship_flames.spread_min = 0;
      ship_flames.spread_max = 40;
      ship_flames.rate = 0.4;
      ship_flames.density = 15;
      ship_flames.size = 10;

      ship_flames.x = x;
      ship_flames.y = y;

      ship_flames.x_min = x;
      ship_flames.x_max = 0;
      ship_flames.y_min = y;
      ship_flames.y_max = 0;

      ship_flames.dx_min = velocity_max;
      ship_flames.dx_max = velocity_max * 2;
      ship_flames.dy_min = velocity_max;
      ship_flames.dy_max = velocity_max * 2;

      ship_flames.rotation = rotation;

      ship_flames.start_colour[0] = 1;
      ship_flames.start_colour[1] = 0;
      ship_flames.start_colour[2] = 0.3;

      ship_flames.gradient_colour[0] = 1;
      ship_flames.gradient_colour[1] = 1;
      ship_flames.gradient_colour[2] = 0;

      ship_flames.offset_x = -size/8;

      return ship_flames;
    }

    void reset(arena_t arena) {
      
      forward = false;
      left = false;
      right = false;
      firing = false;

      x = (rand() % (int) (arena.width / 4)) + arena.width/9;
      y = (rand() % (int) (arena.height / 4)) + arena.height/9;

      rotation = 45;

      dx = 0;
      dy = 0;      
    }

    void update_trail(ParticleGenerator &generator) {
      generator.active = forward;
      generator.x = x;
      generator.x_min = x;
      generator.y = y;
      generator.y_min = y;
      generator.rotation = rotation;
    }

} spaceship;

#endif