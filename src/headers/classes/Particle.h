#ifndef H_PARTICLE
#define H_PARTICLE

class Particle_t {
  public:
    
    // Position & Movement
    float x = 0;
    float y = 0;
    float dx = 0;
    float dy = 0;
    float size = 1;
    float rotation = 0;

    // Time
    float lifetime = 0.1;
    float remaining = 0.1;

    float colour [3] = { 1.0, 0.0, 0.0 };

};

#endif