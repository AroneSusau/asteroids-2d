#ifndef H_BLACKHOLE
#define H_BLACKHOLE

#include "../standard.h"
#include "../settings/game_settings.h"
#include "Util.h"
#include "ParticleGenerator.h"

#define RING_COUNT 25

class BlackHole {

  public:
    
    // Position & Movement
    float x = 0;
    float y = 0;
    float dr = 2;
    float d_growth = 40;
    float pull_x = 1;
    float pull_y = 1;
    
    // Rings
    float size = 140;
    float rings = RING_COUNT;
    float ring_points = 8;
    float ring_positions [RING_COUNT];
    float ring_rotations [RING_COUNT];
    float ring_directions [RING_COUNT];
    float ring_speed [RING_COUNT];
    float ring_colours [RING_COUNT][3];

    static BlackHole default_blackhole() {
      BlackHole b;

      for (int i = 0; i < b.rings; ++i) {
        b.ring_positions[i] = util.random(0, b.size);
        b.ring_rotations[i] = util.random(0, 360);
        b.ring_speed[i] = util.random(b.d_growth/3, b.d_growth);
        b.ring_directions[i] = -1;

        float red = util.random(4, 8) / 10;
        float green = util.random(2, 7) / 10;
        float blue = util.random(4, 10) / 10;
        b.ring_colours[i][0] = red;
        b.ring_colours[i][1] = green;
        b.ring_colours[i][2] = blue;
      }

      return b;
    }

    ParticleGenerator default_particle_generator() {
      ParticleGenerator generator;

      generator.active = true;
      generator.tag = black_hole;
      generator.duration_type = continuous;
      generator.particle_lifetime = 0.2;
      generator.spread_min = 0;
      generator.spread_max = 360;
      generator.rate = 0.2;
      generator.density = 10;
      generator.size = 1;

      generator.x = x;
      generator.y = y;

      generator.x_min = x;
      generator.x_max = x;
      generator.y_min = y;
      generator.y_max = y;

      generator.dx_min = 5;
      generator.dx_max = 25;
      generator.dy_min = 5;
      generator.dy_max = 25;

      float red1 = util.random(4, 8) / 10;
      float green1 = util.random(2, 10) / 10;
      float blue1 = util.random(4, 10) / 10;

      float red2 = util.random(4, 8) / 10;
      float green2 = util.random(2, 10) / 10;
      float blue2 = util.random(4, 10) / 10;

      generator.start_colour[0] = red1;
      generator.start_colour[1] = green1;
      generator.start_colour[2] = blue1;

      generator.gradient_colour[0] = red2;
      generator.gradient_colour[1] = green2;
      generator.gradient_colour[2] = blue2;

      return generator;
    }

};

#endif