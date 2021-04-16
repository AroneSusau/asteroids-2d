#ifndef H_GRAPHICS_RENDERER
#define H_GRAPHICS_RENDERER

#include "../standard.h"
#include "./Util.h"
#include "../settings/game_settings.h"
#include "./Spaceship.h"
#include "./Wall.h"

class GraphicsRenderer_t {
  
  public:
  
    game_window_t game_window;
    arena_t arena;

    void walls(Wall walls [4]);
    void spaceship(spaceship_t spaceship);

};

#endif