#ifndef H_BULLET
#define H_BULLET

class Bullet {

  public:
    
    // Position & Movement
    float x = 0;
    float y = 0;
    float dx = 20;
    float dy = 20;
    float velocity_max = 20;

    // Misc
    bool in_bounds = true;
    float damage = 100;

    float size = 6;
    float colour [3] = { 1.0, 1.0, 1.0 };
    
};

#endif