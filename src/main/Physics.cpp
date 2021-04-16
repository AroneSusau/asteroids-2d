#include "../headers/classes/Physics.h"

void Physics_t::move_ship(spaceship_t &spaceship) {
  float rads = util.deg_to_rad(spaceship.rotation);
  float x_accl = cos(rads) * spaceship.acceleration;
  float y_accl = sin(rads) * spaceship.acceleration;
  float r_accl = spaceship.r_acceleration;

  if (spaceship.dx > spaceship.velocity_max) {
    spaceship.dx = spaceship.velocity_max;
    x_accl = 0;
  } else if (spaceship.dx < -spaceship.velocity_max) {
    spaceship.dx = -spaceship.velocity_max;
    x_accl = 0;
  }

  if (spaceship.dy > spaceship.velocity_max) {
    spaceship.dy = spaceship.velocity_max;
    y_accl = 0;
  } else if (spaceship.dy < -spaceship.velocity_max) {
    spaceship.dy = -spaceship.velocity_max;
    y_accl = 0;
  }

  if (spaceship.dr > spaceship.rotation_max) {
    spaceship.dr = spaceship.rotation_max;
    r_accl = 0;
  } else if (spaceship.dr < -spaceship.rotation_max) {
    spaceship.dr = -spaceship.rotation_max;
    r_accl = 0;
  }
  
  if (spaceship.forward) {
    spaceship.dx += x_accl;
    spaceship.dy += y_accl; 
  }

  if (spaceship.left) {
    spaceship.dr += r_accl;
  }
  
  if (spaceship.right) {
    spaceship.dr -= r_accl;
  }

  if (!spaceship.left && !spaceship.right) {
    if (spaceship.dr > 0) {
      spaceship.dr -= r_accl * 0.8;
    } else if (spaceship.dr < 0) {
      spaceship.dr += r_accl * 0.8;
    }
  }

  spaceship.rotation += spaceship.dr;
  spaceship.x += spaceship.dx;
  spaceship.y += spaceship.dy;

}

void Physics_t::ship_wall_warning(spaceship_t &spaceship, Wall walls [4], arena_t arena) {
  for (int i = 0; i < 4; ++i) {
    if (i % 2 == 0) {
      if (abs(spaceship.y - walls[i].y1) < arena.height * walls[i].warning_distance) {
        walls[i].setRed();
      } else {
        walls[i].setWhite();
      }
    } else {
      if (abs(spaceship.x - walls[i].x1) < arena.width * walls[i].warning_distance) {
        walls[i].setRed();
      } else {
        walls[i].setWhite();
      }
    }
  }
}