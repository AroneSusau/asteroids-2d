#include "../headers/classes/GraphicsRenderer.h"

float pos = 20;

void GraphicsRenderer_t::render(game_window_t game_window) {
  
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  glColor3f(1.0, 1.0, 1.0);
  glTranslatef(game_window.width/2, game_window.height/2, 0.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2d(-pos, -pos);
  glVertex2d(0, 0);
  glVertex2d(pos, -pos);
  glVertex2d(0, pos);
  glEnd();
  
  glPopMatrix();

}