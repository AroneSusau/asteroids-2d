#include "../headers/classes/GraphicsRenderer.h"
#include <random>

void GraphicsRenderer_t::spaceship(spaceship_t spaceship) {
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  glTranslatef(spaceship.x, spaceship.y, 0.0f);
  glRotatef(spaceship.rotation, 0, 0, 1);
  glLineWidth(1.0);

  if (spaceship.fill_style == wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(spaceship.outline_colour[0], spaceship.outline_colour[1], spaceship.outline_colour[2]);
  } else if (spaceship.fill_style == solid) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(spaceship.fill_colour[0], spaceship.fill_colour[1], spaceship.fill_colour[2]);
  }

  glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(-spaceship.size, -spaceship.size);
    glVertex2d(spaceship.size + spaceship.padding, 0);
    glVertex2d(0, 0);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(-spaceship.size, spaceship.size);
    glVertex2d(spaceship.size + spaceship.padding, 0);
    glVertex2d(0, 0);
  glEnd();

  if (spaceship.debug_bounds) {
    glColor3f(spaceship.debug_colour[0], spaceship.debug_colour[1], spaceship.debug_colour[2]);
    glBegin(GL_LINE_LOOP);
      for (int i = 0; i < 32; ++i) {
        float theta = (360 / 32) * i;
        float radian = util.deg_to_rad(theta);
        float x = cosf(radian) * spaceship.bound_radius;
        float y = sinf(radian) * spaceship.bound_radius;
        glVertex2d(x, y);
      }
    glEnd();
  }
  
  glPopMatrix();
}

void GraphicsRenderer_t::asteroids(std::vector<Asteroid> asteroids) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  for (size_t i = 0; i < asteroids.size(); ++i) {
    
    Asteroid asteroid = asteroids.at(i);
    glPushMatrix();

    glTranslatef(asteroid.x, asteroid.y, 0.0f);
    glRotatef(asteroid.rotation, 0, 0, 1);
    glColor3f(asteroid.colour[0], asteroid.colour[1], asteroid.colour[2]);
    glBegin(GL_LINE_LOOP);
      for (int j = 0; j < asteroid.edge_points; ++j) {
        float theta = (360 / asteroid.edge_points) * j;
        float radian = util.deg_to_rad(theta);
        float x = cosf(radian) * asteroid.size * asteroid.edge_variation[j];
        float y = sinf(radian) * asteroid.size * asteroid.edge_variation[j];
        glVertex2d(x, y);
      }
    glEnd();
    glPopMatrix();
  }
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

void GraphicsRenderer_t::debug_line(float x1, float x2, float y1, float y2) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3f(0.0, 0.4, 0.0);

  glPushMatrix();
  glTranslatef(x1, y1, 0);

  glBegin(GL_LINES);
    glVertex2d(0, 0);
    glVertex2d(x2 - x1, y2 - y1);
  glEnd(); 

  glPopMatrix();
}

void GraphicsRenderer_t::bullets(std::vector<Bullet> bullets) {
  
  for (size_t i = 0; i < bullets.size(); ++i) {
    Bullet bullet = bullets.at(i);
    
    glPointSize(bullet.size);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(bullet.colour[0], bullet.colour[1], bullet.colour[2]);

    glPushMatrix();
    glTranslatef(bullet.x, bullet.y, 0);

    glBegin(GL_POINTS);
      glVertex2d(0, 0);
    glEnd(); 

    glPopMatrix();
  }
}

void  GraphicsRenderer_t::hud(arena_t arena) {
  
  std::string score = "Score: " + std::to_string((int) arena.player_points) + " - Next Wave: " + std::to_string((int) arena.next_wave % 60);
  std::string time = "Time: " + std::to_string((int) arena.next_wave % 60);

  glColor3f(0.0, 1.0, 0.0); 
  glRasterPos2f(30, arena.height - 50);

  for (int i = 0; i < score.length(); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score[i]);
  }

  glRasterPos2f(arena.width - 110, arena.height - 50);

  for (int i = 0; i < time.length(); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, time[i]);
  }

}
