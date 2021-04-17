#ifndef H_GRAPHICS_RENDERER
#define H_GRAPHICS_RENDERER

#include "../standard.h"
#include "./Util.h"
#include "../settings/game_settings.h"
#include "./Spaceship.h"
#include "./Asteroid.h"
#include "./Bullet.h"
#include "./Wall.h"

class GraphicsRenderer_t {
  
  public:
  
    game_window_t game_window;
    arena_t arena;

    void walls(Wall walls [4]);
    void spaceship(spaceship_t spaceship);
    void asteroids(std::vector<Asteroid> asteroids);
    void bullets(std::vector<Bullet> bullets);

    void hud(arena_t arena);

    void debug_line(float x1, float x2, float y1, float y2);

};

#endif