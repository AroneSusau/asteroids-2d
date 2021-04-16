#include "../headers/AsteroidGame.h"
#include "../headers/classes/GraphicsRenderer.h"
#include "../headers/classes/Spaceship.h"
#include "../headers/classes/Physics.h"

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

  physics.move_ship(spaceship);
  physics.ship_wall_warning(spaceship, walls, arena);

  glutPostRedisplay();
}

void AsteroidGame::init_game_objs() {

  // Set default variables for graphics renderer
  graphics.arena = arena;
  graphics.game_window = game_window;

  // Spaceship starting position
  spaceship.x = arena.width / 4;
  spaceship.y = arena.height / 4;

  // Wall positions
  int padding = 8;
  walls[0].setPoints(padding, padding, arena.width - padding, padding);
  walls[1].setPoints(arena.width - padding, padding, arena.width - padding, arena.height - padding);
  walls[2].setPoints(arena.width - padding, arena.height - padding, padding, arena.height - padding);
  walls[3].setPoints(padding, arena.height - padding, padding, padding);
}

void AsteroidGame::display() {
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  graphics.walls(walls);
  graphics.spaceship(spaceship);

  int err;
  if ((err = glGetError()) != GL_NO_ERROR)
    fprintf(stderr, "Error: %s\n", gluErrorString(err));

  glutSwapBuffers();
}

void AsteroidGame::on_reshape(int w, int h) {
  
  game_window.resize(w, h);
  glViewport(0, 0, w, h);
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
