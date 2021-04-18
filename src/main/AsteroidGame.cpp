#include "../headers/AsteroidGame.h"

std::vector<ParticleGenerator> particles_generators;
std::vector<Asteroid> asteroids;
std::vector<Bullet> bullets;

BlackHole blackhole;

Wall walls [4];
arena_t arena;
game_window_t game_window;

GraphicsRenderer_t graphics;
Physics_t physics;

void AsteroidGame::init(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("s3728977 - Asteroid");
  
  glutFullScreen();

  glutIgnoreKeyRepeat(GLUT_KEY_REPEAT_OFF);
  
  glutKeyboardFunc(AsteroidGame::on_key_press);
  glutKeyboardUpFunc(AsteroidGame::on_key_release);

  glutMouseFunc(AsteroidGame::on_mouse_event);

  glutDisplayFunc(AsteroidGame::display);
  glutReshapeFunc(AsteroidGame::on_reshape);

  glutIdleFunc(AsteroidGame::on_idle);

  spaceship.x = arena.width/2;
  spaceship.y = arena.height/2;

}

void AsteroidGame::on_mouse_event(int button, int state, int x, int y) {
  if (button == keys.MOUSE_1 && state == keys.MOUSE_DOWN) {  
    spaceship.firing = true;
  } else if (button == keys.MOUSE_1 && state == keys.MOUSE_UP) {
    spaceship.firing = false;
  }
}

void AsteroidGame::generate_asteroid() {
  Asteroid asteroid;
  
  float x_range = util.random(0, arena.spawn_radius - arena.width);
  float y_range = util.random(0, arena.spawn_radius - arena.height);

  asteroid.size = util.random(asteroid.min_size, asteroid.max_size);
  asteroid.initial_health = asteroid.size * 5;
  asteroid.health = asteroid.initial_health;
  asteroid.points = asteroid.size * 10;

  asteroid.bound_radius = asteroid.size * 0.8;
  
  bool findingStart = true;

  while (findingStart) {
    asteroid.x = rand() % 2 == 0 ? -x_range : x_range + arena.width;
    asteroid.y = rand() % 2 != 0 ? -y_range : y_range + arena.height;
    
    if (!asteroids.empty()) {
      for (size_t i = 0; i < asteroids.size(); ++i) {
        Asteroid other = asteroids.at(i);
        bool match = physics.circlular_collision(
          asteroid.x, 
          asteroid.y, 
          asteroid.bound_radius, 
          other.x, 
          other.y, 
          other.bound_radius);

        if (!match && i == asteroids.size() - 1) {
          findingStart = false;
        }
      }
    } else {
      findingStart = false;
    }
  }

  asteroid.dr = util.random(1, 5);
  
  for (int i = 0; i < asteroid.edge_points; ++i) {
    asteroid.edge_variation[i] = util.random(40, 100) / 100;
  }

  float soh = (asteroid.y - spaceship.y) / util.hypo(asteroid.x, asteroid.y, spaceship.x, spaceship.y);
  float cah = (asteroid.x - spaceship.x) / util.hypo(asteroid.x, asteroid.y, spaceship.x, spaceship.y);
  float rand_vel = util.random(asteroid.velocity_min, asteroid.velocity_max);
  
  asteroid.dx = rand_vel * -cosf(acos(cah));
  asteroid.dy = rand_vel * -sinf(asinf(soh));

  asteroid.r_direction = rand() % 2 == 0 ? clockwise : counter_clockwise;
 
  asteroids.push_back(asteroid);
}

void AsteroidGame::on_idle() {

  physics.cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  float dt = physics.cur_time - physics.time;
  
  if (arena.game_started && !arena.game_over) {
    arena.time += dt;
  }

  physics.move_ship(spaceship);
  physics.move_asteroids(asteroids, arena);
  physics.move_bullet(bullets, arena);
  physics.move_black_hole_rings(blackhole, dt);
  
  physics.ship_wall_warning(spaceship, walls, arena);
  physics.ship_wall_collision(spaceship, arena);
  physics.asteroids_wall_collision(asteroids, arena);
  physics.asteroid_to_asteroid_collision(asteroids);

  physics.blackhole_to_ship_collision(blackhole, spaceship, arena);
  physics.blackhole_to_asteroid_collision(blackhole, asteroids, particles_generators);
  
  if (arena.game_started && !arena.game_over) {
    physics.ship_asteroid_collision(asteroids, spaceship, arena);
    physics.bullet_asteroid_collision(bullets, asteroids, particles_generators, arena);
  }

  update_particle_generators(dt);

  if ((arena.game_started && !arena.game_over) || (!arena.game_started && asteroids.size() < arena.asteroid_start_screen_limit)) {
    spawn_asteroids(dt);
  }
  
  spawn_bullets(dt);

  if ((!arena.in_bounds || !arena.player_alive)) {
    
    if (!arena.game_started) {

    } else if (!arena.game_over) {
      init_game_objs();
    } else {
      spaceship.x = arena.width/2;
      spaceship.y = arena.height/2;
    }
  }

  glutPostRedisplay();
}

void AsteroidGame::spawn_asteroids(float dt) {
  
  arena.next_wave -= dt;
  
  if (arena.next_wave <= 0) {
    
    for (int i = 0; i < arena.spawn; ++i) {
      generate_asteroid();  
    }

    arena.next_wave = arena.wave_interval;
    arena.spawn += arena.spawn_increment;
  }
}

void AsteroidGame::spawn_bullets(float dt) {

  if (spaceship.firing) {
    spaceship.next_fire -= dt;

    if (spaceship.next_fire <= 0) {
      Bullet bullet;
      
      bullet.x = spaceship.x + spaceship.size * cos(util.deg_to_rad(spaceship.rotation));
      bullet.y = spaceship.y + spaceship.size * sin(util.deg_to_rad(spaceship.rotation));

      bullet.dx = bullet.velocity_max * cos(util.deg_to_rad(spaceship.rotation));
      bullet.dy = bullet.velocity_max * sin(util.deg_to_rad(spaceship.rotation));

      bullets.push_back(bullet);
      spaceship.next_fire = spaceship.fire_rate;
    }
  }
}

void AsteroidGame::update_particle_generators(float dt) {
  for (size_t i = 0; i < particles_generators.size(); ++i) {
    ParticleGenerator &generator = particles_generators.at(i);
    
    if (generator.tag == ship_trail && !arena.game_over) {
      spaceship.update_trail(generator);
    } else if (generator.tag == ship_trail && arena.game_over) {
      particles_generators.erase(particles_generators.begin() + i);
    }

    generator.update_lifetime(dt);
    generator.movement(dt);

    if (generator.active) {
      
      generator.spawn(dt);

    } else if (generator.tag != ship_trail) {
      particles_generators.erase(particles_generators.begin() + i);  
    }
  }
}

void AsteroidGame::init_game_objs() {

  // Set initial time
  physics.time = glutGet(GLUT_ELAPSED_TIME) / 1000;

  // Default settings
  spaceship.reset(arena);
  blackhole = blackhole.default_blackhole();
  
  arena.in_bounds = true;
  arena.player_alive = true;
  arena.next_wave = 0;
  arena.spawn = 1;
  arena.time = 0;
  arena.player_points = 0;

  // Setup blackhole
  blackhole.x = util.random(arena.width/4 + - blackhole.size, arena.width - blackhole.size);
  blackhole.y = util.random(arena.height/4 + blackhole.size, arena.height - blackhole.size);

  // Set default variables for graphics renderer
  graphics.arena = arena;
  graphics.game_window = game_window;

  // Wall positions
  int padding = 8;
  walls[0].setPoints(padding, padding, arena.width - padding, padding);
  walls[1].setPoints(arena.width - padding, padding, arena.width - padding, arena.height - padding);
  walls[2].setPoints(arena.width - padding, arena.height - padding, padding, arena.height - padding);
  walls[3].setPoints(padding, arena.height - padding, padding, padding);

  // Default Particle Generators
  for (size_t i = 0; i < particles_generators.size(); ++i) {
    if (particles_generators.at(i).tag != environment) {
      particles_generators.erase(particles_generators.begin() + i);
    }
  }

  asteroids.clear();
  bullets.clear();

  particles_generators.push_back(spaceship.default_trail());
  particles_generators.push_back(blackhole.default_particle_generator());
}

void AsteroidGame::display() {
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Render particle generators
  for (size_t i = 0; i < particles_generators.size(); ++i) {
    particles_generators.at(i).render();
  }

  // General rendering
  graphics.walls(walls);
  graphics.asteroids(asteroids);
  graphics.blackhole(blackhole);

  if (arena.game_started && !arena.game_over) {
    graphics.spaceship(spaceship);
    graphics.bullets(bullets);
    graphics.hud(arena);
  } else if (!arena.game_started) {
    graphics.start_screen(arena);
  } else if (arena.game_started && arena.game_over) {
    graphics.game_over(arena);
  } 

  int err;
  if ((err = glGetError()) != GL_NO_ERROR)
    fprintf(stderr, "Error: %s\n", gluErrorString(err));

  glutSwapBuffers();
}

void AsteroidGame::on_reshape(int w, int h) {
  
  glViewport(0, 0, w, h);
  game_window.resize(w, h);
  arena.resize(w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0, arena.width, 0.0, arena.height, -1, 1);
  
  particles_generators.push_back(arena.default_stars());
}

void AsteroidGame::on_key_press(unsigned char key, int x, int y) {
  if (arena.game_started && !arena.game_over) {
    if (key == keys.W) {
      spaceship.forward = true;
    } else if (key == keys.A) {
      spaceship.left = true;
    } else if (key == keys.D) {
      spaceship.right = true;
    } else if (key == keys.SPACE) {
      spaceship.firing = true;
    }
  } else {
    arena.game_started = true;
    arena.game_over = false;
    init_game_objs();
  }

  if (key == keys.ESC) {
    exit(EXIT_SUCCESS);
  }
}

void AsteroidGame::on_key_release(unsigned char key, int x, int y) {
  if (arena.game_started && !arena.game_over) {
    if (key == keys.W) {
      spaceship.forward = false;
    } else if (key == keys.A) {
      spaceship.left = false;
    } else if (key == keys.D) {
      spaceship.right = false;
    } else if (key == keys.SPACE) {
      spaceship.firing = false;
    } 
  }
}

AsteroidGame::AsteroidGame(/* args */)
{}

AsteroidGame::~AsteroidGame()
{}

void AsteroidGame::run(int argc, char** argv) {
  AsteroidGame::init(argc, argv);
  glutMainLoop();
}
