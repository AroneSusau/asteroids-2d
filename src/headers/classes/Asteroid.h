#ifndef H_ASTEROID
#define H_ASTEROID

typedef enum { clockwise, counter_clockwise } rotation_direction;

class Asteroid {

  public:
    
    // Position & Movement
    float x = 0;
    float y = 0;
    float dx = 0;
    float dy = 0;
    float dr = 0;
    float velocity_min = 4;
    float velocity_max = 10;
    float rotation = 0;
    bool entered_arena = false;
    rotation_direction r_direction = clockwise;

    // Bounds & Edges
    float bound_radius = 6;
    float edge_points = 8;
    float edge_variation [8];

    // Life Stats
    float initial_health = 100;
    float health = 100;
    float points = 1;
    bool hasSplit = false;

    // Characteristics
    float size = 5;
    float colour [3] = { 1.0, 1.0, 1.0 };

    bool debug = false;

    ParticleGenerator explosion() {
      ParticleGenerator generator;

      generator.duration = 0.3;
      generator.remaining = 0.3;

      generator.active = true;
      generator.tag = asteroid_burst;
      generator.duration_type = discrete;
      generator.particle_lifetime = 0.1;
      generator.spread_min = 0;
      generator.spread_max = 360;
      generator.rate = 0.2;
      generator.density = 4;
      generator.size = 9;

      generator.x = x;
      generator.y = y;

      generator.x_min = x;
      generator.x_max = bound_radius;
      generator.y_min = y;
      generator.y_max = bound_radius;

      generator.dx_min = 1;
      generator.dx_max = 25;
      generator.dy_min = 1;
      generator.dy_max = 25;

      // generator.rotation = rotation;

      generator.start_colour[0] = 0;
      generator.start_colour[1] = 0;
      generator.start_colour[2] = 1;

      generator.gradient_colour[0] = 0.7;
      generator.gradient_colour[1] = 1;
      generator.gradient_colour[2] = 1;

      generator.active = true;
      generator.duration_type = discrete;
      generator.tag = asteroid_burst;

      return generator;
    }

};

#endif