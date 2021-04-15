#include "../headers/settings/game_settings.h"

#include <iostream>

void arena_t::resize(float o_width, float o_height) {
    
  if (this->width > o_width * 0.9) {
    this->width = o_width - (o_width * this->margin);
  }

  if (this->height > o_height * 0.9) {
    this->height = o_height - (o_height * this->margin);
  }

}

void game_window_t::resize(float width, float height) {
  this->width = width;
  this->height = height;
}