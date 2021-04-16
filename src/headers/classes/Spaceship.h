#ifndef H_SPACESHIP
#define H_SPACESHIP

typedef enum { solid, wireframe } ship_fill;

static class spaceship_t {
  public:

    // Position & Movement
    float x;
    float y;
    float dx = 0;
    float dy = 0;
    float velocity_max = 7;
    float rotation_max = 3;
    float acceleration = 0.4;
    float r_acceleration = 0.2;

    float rotation = 0;
    float dr = 0;

    float bound_radius = 28.28;
    
    // Size
    float size = 20;
    float padding = 8;

    // State
    bool forward = false;
    bool left = false;
    bool right = false;

    // Fill & colour
    float fill_colour [3] = { 0.0, 1.0, 0.0 };
    float outline_colour [3] = { 1.0, 1.0, 1.0 };
    float debug_colour [3] = { 0.0, 0.0, 0.6 };
    ship_fill fill_style = wireframe;

    // Debug
    bool debug_bounds = true;
} spaceship;

#endif