#ifndef H_PARTICLE_GENERATOR
#define H_PARTICLE_GENERATOR

#include "../standard.h"
#include "./Particle.h"
#include "./Util.h"

typedef enum { discrete, continuous } duration_t;
typedef enum { ship_trail, asteroid_burst, misc } generator_t;

class ParticleGenerator {
  public:
    
    generator_t tag = misc;
    std::vector<Particle_t> particles;

    // Lifetime
    duration_t duration_type = discrete;
    bool active = true;
    float duration = 1;
    float remaining = 1;
    float particle_lifetime = 1;

    // Characteristics
    int spread_min = 0;
    int spread_max = 0;
    float rate = 1;
    float density = 1;
    float size = 1;

    // Position & Movement
    float x = 0;
    float y = 0;

    float x_min = 0;
    float x_max = 0;

    float y_min = 0;
    float y_max = 0;

    int dx_max = 2;
    int dx_min = 1;
    int dy_max = 2;
    int dy_min = 1;
    float rotation = 0;

    float offset_x = 0;
    float offset_y = 0;

    // Colour
    float start_colour [3] = { 1.0, 1.0, 1.0 };
    float gradient_colour [3] = { 1.0, 1.0, 1.0 };

    void spawn(float dt);
    void create_particle();
    void movement(float dt);
    void update_lifetime(float dt);
    void update_colour(Particle_t &particle);
    void render();
};

#endif