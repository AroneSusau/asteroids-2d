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
  } else {
    spaceship.dx *= 0.96;
    spaceship.dy *= 0.96;
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

    if (asteroid.dx > asteroid.velocity_max) {
      asteroid.dx = asteroid.velocity_max;
    }

    if (asteroid.dx < -asteroid.velocity_max) {
      asteroid.dx = -asteroid.velocity_max;
    }

    if (asteroid.dy > asteroid.velocity_max) {
      asteroid.dy = asteroid.velocity_max;
    }

    if (asteroid.dy < -asteroid.velocity_max) {
      asteroid.dy = -asteroid.velocity_max;
    }

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

          if (!asteroid.hasSplit) {
            
            for (int i = 0; i < 2; ++i) {
              Asteroid a;
              int side = i == 0 ? 1 : -1;
              float ratio = 0.9;
              
              a.x = asteroid.x + side * asteroid.size/2;
              a.y = asteroid.y + side * asteroid.size/2;
              a.dx = asteroid.dx * -side;
              a.dy = asteroid.dy * -side;
              a.bound_radius = asteroid.bound_radius/2;
              a.dr = asteroid.dr;
              a.r_direction = asteroid.r_direction;
              a.rotation = asteroid.rotation;
              a.size = asteroid.size/2 * ratio;
              a.hasSplit = true;
              a.points = asteroid.points / 2;
              a.initial_health = asteroid.initial_health / 2;
              a.health = asteroid.health / 2;          
              
              for (int i = 0; i < asteroid.edge_points; ++i) {
                a.edge_variation[i] = asteroid.edge_variation[i];
              }

              asteroids.push_back(a);
            }
          }

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

    if (circlular_collision(asteroid.x, asteroid.y, asteroid.bound_radius, spaceship.x, spaceship.y, spaceship.bound_radius)) {
      arena.player_alive = false;
      arena.game_over = true;
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
    arena.game_over = true;
  }
}

void Physics_t::asteroid_to_asteroid_collision(std::vector<Asteroid> &asteroids) {
  for (size_t i = 0; i < asteroids.size(); ++i) {
    Asteroid &a1 = asteroids.at(i);
    
    for (size_t j = i + 1; j < asteroids.size(); ++j) {
      Asteroid &a2 = asteroids.at(j);

      if (circlular_collision(a1.x, a1.y, a1.bound_radius, a2.x, a2.y, a2.bound_radius)) {
        
        float factor = 1000;
        float newdx1 = elastic_collsion(a1.dx, a2.dx, a1.size / factor, a2.size / factor);
        float newdy1 = elastic_collsion(a1.dy, a2.dy, a1.size / factor, a2.size / factor);

        float newdx2 = elastic_collsion(a2.dx, a1.dx, a1.size / factor, a2.size / factor);
        float newdy2 = elastic_collsion(a2.dy, a1.dy, a1.size / factor, a2.size / factor);

        a1.dx = newdx1;
        a1.dy = newdy1;
        a2.dx = newdx2;
        a2.dy = newdy2;

        a1.x += a1.dx;
        a1.y += a1.dy;
        a2.x += a2.dx;
        a2.y += a2.dy;
      }
    }
  }
}

void Physics_t::move_black_hole_rings(BlackHole &hole, float dt) {

  for(int i = 0; i < hole.rings; ++i) {
    
    hole.ring_positions[i] += hole.ring_speed[i] * hole.ring_directions[i] * dt;
    hole.ring_rotations[i] += hole.dr * dt;
    
    if (hole.ring_positions[i] >= hole.size) {
      hole.ring_directions[i] = -1;
    } else if (hole.ring_positions[i] <= 0) {
      hole.ring_directions[i] = 1;
    } 
  }
}

void Physics_t::blackhole_to_asteroid_collision(BlackHole hole, std::vector<Asteroid> &asteroids, std::vector<ParticleGenerator> &particles_generators) {
  for (size_t i = 0; i < asteroids.size(); ++i) {
    Asteroid &asteroid = asteroids.at(i);
    
    if (circlular_collision(hole.x, hole.y, hole.size, asteroid.x, asteroid.y, asteroid.bound_radius)) {
      particles_generators.push_back(asteroid.explosion());
      asteroids.erase(asteroids.begin() + i);
    }
  }
}

void Physics_t::blackhole_to_ship_collision(BlackHole hole, spaceship_t &spaceship, arena_t &arena) {
  if (circlular_collision(hole.x, hole.y, hole.size, spaceship.x, spaceship.y, spaceship.bound_radius)) {
    arena.player_alive = false;
    arena.game_over = true;
  }
}

float Physics_t::elastic_collsion(float dv1, float dv2, float m1, float m2) {
  return (dv1 * (m1 - m2)) + ((2 * m2 * dv2) / (m1 + m2));
}

bool Physics_t::circlular_collision(float x1, float y1, float r1, float x2, float y2, float r2) {
  float diff_x = x1 - x2;
  float diff_y = y1 - y2;
  float hypo = abs(sqrt(pow(diff_y, 2) + pow(diff_x, 2)));
  float bounds = r1 + r2;

  return hypo < bounds;
}

void Physics_t::blackhole_pull_effect(BlackHole hole, spaceship_t &spaceship, std::vector<Asteroid> &asteroids) {

  float hypo = util.hypo(hole.x, hole.y, spaceship.x, spaceship.y);
  float soh = (hole.y - spaceship.y) / hypo;
  float cah = (hole.x - spaceship.x) / hypo;
  
  spaceship.dx += hole.pull_x * cosf(acos(cah)) * sqrt(M_PI / hypo);
  spaceship.dy += hole.pull_y * sinf(asinf(soh)) * sqrt(M_PI / hypo);

  for (size_t i = 0; i < asteroids.size(); ++i) {
    Asteroid &a = asteroids.at(i);
    
    float hypo = util.hypo(hole.x, hole.y, a.x, a.y);
    float soh = (hole.y - a.y) / hypo;
    float cah = (hole.x - a.x) / hypo;
    
    a.dx += hole.pull_x * cosf(acos(cah)) * sqrt(M_PI / hypo);
    a.dy += hole.pull_y * sinf(asinf(soh)) * sqrt(M_PI / hypo);
  }

}