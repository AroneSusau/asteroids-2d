#include "../headers/settings/game_settings.h"

#include <iostream>

void arena_t::resize(float o_width, float o_height) {
    
  float o_aspect = o_width / o_height;
  float aspect = this->width / this->height;

  if (o_aspect >= aspect) {
    this->width *= o_aspect/aspect;
  } else {
    this->height *= aspect/o_aspect;
  }

}

void game_window_t::resize(float width, float height) {
  this->width = width;
  this->height = height;
}