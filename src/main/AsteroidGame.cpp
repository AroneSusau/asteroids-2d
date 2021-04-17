#include "../headers/AsteroidGame.h"

std::vector<ParticleGenerator> particles_generators;
std::vector<Asteroid> asteroids;
std::vector<Bullet> bullets;

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

}

void AsteroidGame::on_mouse_event(int button, int state, int x, int y) {
  if (button == keys.MOUSE_1 && state == keys.MOUSE_DOWN) {  
    spaceship.firing = true;
  } else if (button == keys.MOUSE_1 && state == keys.MOUSE_UP) {
    spaceship.firing = false;
  }

  if (button == 2 && state == 0) {
    generate_asteroid();
  }
}

void AsteroidGame::generate_asteroid() {
  Asteroid asteroid;
  
  float x_range = util.random(0, arena.spawn_radius - arena.width);
  float y_range = util.random(0, arena.spawn_radius - arena.height);

  asteroid.size = util.random(30, 60);
  asteroid.initial_health = asteroid.size * 5;
  asteroid.health = asteroid.initial_health;
  asteroid.points = asteroid.size * 10;

  asteroid.bound_radius = asteroid.size;
  asteroid.x = rand() % 2 == 0 ? -x_range : x_range + arena.width;
  asteroid.y = rand() % 2 != 0 ? -y_range : y_range + arena.height;

  // DELETE ME
  // asteroid.x = arena.width/2;
  // asteroid.y = arena.height/2;
  // DELETE ME

  asteroid.dr = util.random(1, 5);
  
  for (int i = 0; i < asteroid.edge_points; ++i) {
    asteroid.edge_variation[i] = util.random(60, 100) / 100;
  }

  float soh = (asteroid.y - spaceship.y) / util.hypo(asteroid.x, asteroid.y, spaceship.x, spaceship.y);
  float cah = (asteroid.x - spaceship.x) / util.hypo(asteroid.x, asteroid.y, spaceship.x, spaceship.y);

  asteroid.dx = util.random(asteroid.velocity_min, asteroid.velocity_max) * -cosf(acos(cah));
  asteroid.dy = util.random(asteroid.velocity_min, asteroid.velocity_max) * -sinf(asinf(soh));

  // DELETE ME
  // asteroid.dx = -2;
  // asteroid.dy = -2;
  // DELETE ME

  asteroid.r_direction = rand() % 2 == 0 ? clockwise : counter_clockwise;
 
  asteroids.push_back(asteroid);
}

void AsteroidGame::on_idle() {

  physics.cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  arena.time = physics.cur_time;
  float dt = physics.cur_time - physics.time;

  physics.move_ship(spaceship);
  physics.move_asteroids(asteroids, arena);
  physics.move_bullet(bullets, arena);
  
  physics.ship_wall_warning(spaceship, walls, arena);
  physics.ship_wall_collision(spaceship, arena);
  physics.asteroids_wall_collision(asteroids, arena);
  physics.ship_asteroid_collision(asteroids, spaceship, arena);
  physics.bullet_asteroid_collision(bullets, asteroids, particles_generators, arena);

  update_particle_generators(dt);

  spawn_asteroids(dt);
  spawn_bullets(dt);

  if (!arena.in_bounds || !arena.player_alive) {
    init_game_objs();
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
    
    if (generator.tag == ship_trail) {
      spaceship.update_trail(generator);
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
  arena.in_bounds = true;
  arena.player_alive = true;
  arena.next_wave = 0;
  arena.spawn = 1;

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

  particles_generators.push_back(arena.default_stars());
  particles_generators.push_back(spaceship.default_trail());
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
  graphics.spaceship(spaceship);
  graphics.asteroids(asteroids);
  graphics.bullets(bullets);
  graphics.hud(arena);

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
  
  init_game_objs();
}

void AsteroidGame::on_key_press(unsigned char key, int x, int y) {
  if (key == keys.W) {
    spaceship.forward = true;
  } else if (key == keys.A) {
    spaceship.left = true;
  } else if (key == keys.D) {
    spaceship.right = true;
  } else if (key == keys.SPACE) {
    spaceship.firing = true;
  } else if (key == keys.ESC) {
    exit(EXIT_SUCCESS);
  }
}

void AsteroidGame::on_key_release(unsigned char key, int x, int y) {
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

AsteroidGame::AsteroidGame(/* args */)
{}

AsteroidGame::~AsteroidGame()
{}

void AsteroidGame::run(int argc, char** argv) {
  AsteroidGame::init(argc, argv);
  glutMainLoop();
}
