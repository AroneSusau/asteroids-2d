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

void Physics_t::move_asteroids(std::vector<Asteroid> &asteroids, arena_t &arena) {

  for (size_t i = 0; i < asteroids.size(); ++i) {

    Asteroid &asteroid = asteroids.at(i);

    asteroid.x += asteroid.dx;
    asteroid.y += asteroid.dy;
    
    asteroid.rotation += asteroid.r_direction == clockwise ? -asteroid.dr : asteroid.dr;

  }
}

void Physics_t::move_bullet(std::vector<Bullet> &bullets, arena_t &arena) {

  for (size_t i = 0; i < bullets.size(); ++i) {
    Bullet &bullet = bullets.at(i);

    bullet.x += bullet.dx;
    bullet.y += bullet.dy;

    bullet.in_bounds = 
      bullet.x > 0 && 
      bullet.x < arena.width && 
      bullet.y > 0 && 
      bullet.y < arena.width;

    if (!bullet.in_bounds) {
      bullets.erase(bullets.begin() + i);
    }
  }  
}

void Physics_t::bullet_asteroid_collision(std::vector<Bullet> &bullets, std::vector<Asteroid> &asteroids, std::vector<ParticleGenerator> &particles_generators, arena_t &arena) {
  for (size_t i = 0; i < bullets.size(); ++i) {
    Bullet &bullet = bullets.at(i);
    
    for (size_t j = 0; j < asteroids.size(); ++j) {
      Asteroid &asteroid = asteroids.at(j);

      float diff_x = abs(asteroid.x - bullet.x);
    float diff_y = abs(asteroid.y - bullet.y);

      if (diff_x < asteroid.bound_radius && diff_y < asteroid.bound_radius) {
        bullet.in_bounds = false;
        asteroid.health -= bullet.damage;
        
        if (asteroid.health <= 0) {
          arena.player_points += asteroid.points;
          particles_generators.push_back(asteroid.explosion());
          asteroids.erase(asteroids.begin() + j);
        }

        bullets.erase(bullets.begin() + i);
      }
    }
  }
}

void Physics_t::ship_asteroid_collision(std::vector<Asteroid> &asteroids, spaceship_t &spaceship, arena_t &arena) {

  for (size_t i = 0; i < asteroids.size(); ++i) {

    Asteroid &asteroid = asteroids.at(i);

    float diff_x = abs(asteroid.x - spaceship.x);
    float diff_y = abs(asteroid.y - spaceship.y);
    float bounds = asteroid.bound_radius + spaceship.bound_radius;

    if (diff_x < bounds && diff_y < bounds) {
      arena.player_alive = false;
    }
  } 
}

void Physics_t::asteroids_wall_collision(std::vector<Asteroid> &asteroids, arena_t &arena) {

  for (size_t i = 0; i < asteroids.size(); ++i) {

    Asteroid &asteroid = asteroids.at(i);

    float left = asteroid.x - asteroid.bound_radius < 0;
    float right = asteroid.x + asteroid.bound_radius > arena.width;
    float top = asteroid.y + asteroid.bound_radius > arena.height;
    float bot = asteroid.y - asteroid.bound_radius < 0;

    if (asteroid.entered_arena) {      
      
      if (left || right) {
        asteroid.dx *= -1;
      }

      if (top || bot) {
        asteroid.dy *= -1;
      }

    } else if (
      !left && !right && !top && !bot
    ) {
      asteroid.entered_arena = true;
    }
  }
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
  float bot = spaceship.y - spaceship.bound_radius < 0;
  
  if (left || right || top || bot) {
    arena.in_bounds = false;
  }
}