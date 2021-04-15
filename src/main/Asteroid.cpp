#include "../headers/AsteroidGame.h"
#include "../headers/classes/GraphicsRenderer.h"

void AsteroidGame::init(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutFullScreen(); 

  glutIgnoreKeyRepeat(GLUT_KEY_REPEAT_OFF);
  glutKeyboardFunc(AsteroidGame::on_key_press);

  glutDisplayFunc(AsteroidGame::display);
  glutReshapeFunc(AsteroidGame::on_reshape);
}

void AsteroidGame::display() {
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glColor3f(1.0, 1.0, 1.0);
  glTranslatef(arena.width/2, arena.height/2, 0.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2d(-arena.width/2 + 1, -arena.height/2 + 1);
  glVertex2d(arena.width/2 - 1, -arena.height/2 + 1);
  glVertex2d(arena.width/2 - 1, arena.height/2 - 1);
  glVertex2d(-arena.width/2 + 1, arena.height/2 - 1);
  glEnd();
  glPopMatrix();

  graphics.render(game_window);

  int err;
  if ((err = glGetError()) != GL_NO_ERROR)
    fprintf(stderr, "Error: %s\n", gluErrorString(err));

  glutSwapBuffers();
}

void AsteroidGame::on_reshape(int w, int h) {
  
  game_window.resize(w, h);
  glViewport(0, 0, w, h);
  // arena.resize(w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0, arena.width, 0.0, arena.height, -1, 1);
}

void AsteroidGame::on_key_press(unsigned char key, int x, int y) {
  if (key == keys.escape) exit(EXIT_SUCCESS);
}

AsteroidGame::AsteroidGame(/* args */)
{}

AsteroidGame::~AsteroidGame()
{}

void AsteroidGame::run(int argc, char** argv) {
  AsteroidGame::init(argc, argv);
  glutMainLoop();
}
