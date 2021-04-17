#include "../headers/classes/Physics.h"

void Physics_t::move_ship(spaceship_t &spaceship) {
  float dt = cur_time - time;
  float rads = util.deg_to_rad(spaceship.rotation);
  float x_accl = cos(rads) * spaceship.acceleration * dt;
  float y_accl = sin(rads) * spaceship.acceleration * dt;
  float r_accl = spaceship.r_acceleration * dt;

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
    spaceship.dr *= 0.7;
  }

  spaceship.rotation += spaceship.dr;
  spaceship.x += spaceship.dx;
  spaceship.y += spaceship.dy;

  time = cur_time;
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

void Physics_t::ship_wall_collision(spaceship_t &spaceship, arena_t &arena) {
  
  float left = spaceship.x - spaceship.bound_radius < 0;
  float right = spaceship.x + spaceship.bound_radius > arena.width;
  float top = spaceship.y + spaceship.bound_radius > arena.height;
  float bot = spaceship.y + spaceship.bound_radius < 0;
  
  if (left || right || top || bot) {
    arena.in_bounds = false;
  }
}