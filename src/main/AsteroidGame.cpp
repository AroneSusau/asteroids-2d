#include "../headers/AsteroidGame.h"
#include "../headers/classes/GraphicsRenderer.h"
#include "../headers/classes/Spaceship.h"
#include "../headers/classes/Physics.h"
#include "../headers/classes/ParticleGenerator.h"

std::vector<ParticleGenerator> particles_generators;

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

  glutDisplayFunc(AsteroidGame::display);
  glutReshapeFunc(AsteroidGame::on_reshape);

  glutIdleFunc(AsteroidGame::on_idle);

  init_game_objs();
}

void AsteroidGame::on_idle() {

  physics.cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  float dt = physics.cur_time - physics.time;

  physics.move_ship(spaceship);
  physics.ship_wall_warning(spaceship, walls, arena);
  physics.ship_wall_collision(spaceship, arena);

  update_particle_generators(dt);

  if (!arena.in_bounds) {
    init_game_objs();
  }

  glutPostRedisplay();
}

void AsteroidGame::update_particle_generators(float dt) {
  for (size_t i = 0; i < particles_generators.size(); ++i) {
    ParticleGenerator &generator = particles_generators.at(i);
    
    if (generator.tag == ship_trail) {
      generator.active = spaceship.forward;
      generator.x = spaceship.x;
      generator.x_min = spaceship.x;
      generator.y = spaceship.y;
      generator.y_min = spaceship.y;
      generator.rotation = spaceship.rotation;
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

  spaceship.reset(arena);
  particles_generators.clear();  
  arena.in_bounds = true;

  ParticleGenerator stars;

  stars.active = true;
  stars.duration_type = continuous;
  stars.particle_lifetime = 20;
  stars.spread_min = 90;
  stars.spread_max = 90;
  stars.rotation = 0;
  stars.rate = 0.2;
  stars.density = 3;
  stars.size = 3;

  stars.x = arena.width/2;
  stars.y = arena.height/2;

  stars.x_min = 0;
  stars.y_min = arena.height;

  stars.x_max = arena.width;
  stars.y_max = arena.height;

  stars.dx_min = 0;
  stars.dx_max = 0;
  stars.dy_min = 4;
  stars.dy_max = 4;

  particles_generators.push_back(stars);

  // Set initial time
  physics.time = glutGet(GLUT_ELAPSED_TIME) / 1000;

  // Set default variables for graphics renderer
  graphics.arena = arena;
  graphics.game_window = game_window;

  // Wall positions
  int padding = 8;
  walls[0].setPoints(padding, padding, arena.width - padding, padding);
  walls[1].setPoints(arena.width - padding, padding, arena.width - padding, arena.height - padding);
  walls[2].setPoints(arena.width - padding, arena.height - padding, padding, arena.height - padding);
  walls[3].setPoints(padding, arena.height - padding, padding, padding);

  // Ship Particle Generator
  particles_generators.push_back(spaceship.default_trail());
}

void AsteroidGame::display() {
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  for (size_t i = 0; i < particles_generators.size(); ++i) {
    particles_generators.at(i).render();
  }

  graphics.walls(walls);
  graphics.spaceship(spaceship);

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
