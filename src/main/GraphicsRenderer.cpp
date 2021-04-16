#include "../headers/classes/GraphicsRenderer.h"

int circle_points = 32;

void GraphicsRenderer_t::spaceship(spaceship_t spaceship) {
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  glTranslatef(spaceship.x, spaceship.y, 0.0f);
  glRotatef(spaceship.rotation, 0, 0, 1);
  glLineWidth(1.0);
  
  if (spaceship.fill_style == wireframe) {
    glColor3f(spaceship.outline_colour[0], spaceship.outline_colour[1], spaceship.outline_colour[2]);
    glBegin(GL_LINE_LOOP);
      glVertex2d(-spaceship.size, -spaceship.size);
      glVertex2d(spaceship.size + spaceship.padding, 0);
      glVertex2d(-spaceship.size, spaceship.size);
      glVertex2d(0, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2d(0, 0);
    glVertex2d(spaceship.size + spaceship.padding, 0);
    glEnd();
  } else if (spaceship.fill_style == solid) {
    glColor3f(spaceship.fill_colour[0], spaceship.fill_colour[1], spaceship.fill_colour[2]);
    glBegin(GL_POLYGON);
      glVertex2d(-spaceship.size, -spaceship.size);
      glVertex2d(spaceship.size + spaceship.padding, 0);
      glVertex2d(-spaceship.size, spaceship.size);
      glVertex2d(0, 0);
    glEnd();
  }

  if (spaceship.debug_bounds) {
    glColor3f(spaceship.debug_colour[0], spaceship.debug_colour[1], spaceship.debug_colour[2]);
    glBegin(GL_LINE_LOOP);
      for (int i = 0; i < circle_points; ++i) {
        float theta = (360 / circle_points) * i;
        float radian = util.deg_to_rad(theta);
        float x = cosf(radian) * spaceship.bound_radius;
        float y = sinf(radian) * spaceship.bound_radius;
        glVertex2d(x, y);
      }
    glEnd();
  }
  
  glPopMatrix();
}

void GraphicsRenderer_t::walls(Wall walls [4]) {
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
  glLineWidth(8.0);
  
  for (int i = 0; i < 4; ++i) {
    glColor3f(walls[i].colour[0], walls[i].colour[1], walls[i].colour[2]);
    glBegin(GL_LINES);
      glVertex2d(walls[i].x1, walls[i].y1);
      glVertex2d(walls[i].x2, walls[i].y2);
    glEnd();
  }

  glPopMatrix();

}