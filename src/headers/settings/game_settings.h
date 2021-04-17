#ifndef H_GAME_SETTINGS
#define H_GAME_SETTINGS

#include "../classes/ParticleGenerator.h"

class arena_t {
  public:
    
    bool in_bounds = true;
    bool player_alive = true;
    bool game_over = false;

    // Positions & Size
    float width = 1500;
    float height = 1500;
    float margin = 0.1;

    // Asteroid Spawns
    float next_wave = 0;
    float wave_interval = 10;
    float spawn = 1;
    float spawn_increment = 2;
    float spawn_radius = sqrt(pow(width, 2) + pow(height, 2));
    float player_points = 0;
    float time = 0;

    void resize(float o_width, float o_height);

    ParticleGenerator default_stars() {
      ParticleGenerator stars;

      stars.active = true;
      stars.tag = environment;
      stars.duration_type = continuous;
      stars.particle_lifetime = 20;
      stars.spread_min = 90;
      stars.spread_max = 90;
      stars.rotation = 0;
      stars.rate = 0.2;
      stars.density = 1;
      stars.size = 3;

      stars.x = width/2;
      stars.y = height/2;

      stars.x_min = 0;
      stars.y_min = height;

      stars.x_max = width;
      stars.y_max = height;

      stars.dx_min = 0;
      stars.dx_max = 0;
      stars.dy_min = 4;
      stars.dy_max = 4;

      return stars;
    }

};

class game_window_t {
  public:
    
    float width = 0;
    float height = 0;

    void resize(float width, float height);
    
};

#endif