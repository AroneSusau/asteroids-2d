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
  glLineWidth(1);
  
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

void  GraphicsRenderer_t::start_screen(arena_t arena) {
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  float x1 = (arena.width / 6) * 2;
  float y1 = (arena.height / 6) * 2;

  float x2 = (arena.width / 6) * 4;
  float y2 = (arena.height / 6) * 4;

  glBegin(GL_LINE_LOOP);
    glVertex2d(x1, y1);
    glVertex2d(x2, y1);
    glVertex2d(x2, y2);
    glVertex2d(x1, y2);
  glEnd();

  float window_width = glutGet(GLUT_WINDOW_WIDTH);
  float window_height = glutGet(GLUT_WINDOW_HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, window_width, 0.0, window_height, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  const unsigned char title [42] = "A    S    T    E    R    O    I    D    S";
  const unsigned char subtitle [36] = "Interactive 3d graphics & animation";
  const unsigned char auth [21] = "Arone Susau s3728977";
  const unsigned char msg [23] = "Press any key to begin";
  
  int title_width = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, title) / 2.0;
  
  glColor3f(1.0, 1.0, 1.0); 
  glRasterPos2f(window_width/2 - title_width, window_height / 2 + 50);

  for (int i = 0; i < std::strlen((char*) title); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);
  }

  glRasterPos2f(window_width/2 - title_width, window_height / 2 - 10);

  for (int i = 0; i < std::strlen((char*) subtitle); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, subtitle[i]);
  }

  glRasterPos2f(window_width/2 - title_width, window_height / 2 - 40);

  for (int i = 0; i < std::strlen((char*) auth); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, auth[i]);
  }

  glRasterPos2f(window_width/2 - title_width, window_height / 2 - 90);

  for (int i = 0; i < std::strlen((char*) msg); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, msg[i]);
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

}

void  GraphicsRenderer_t::game_over(arena_t arena) {
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  float x1 = (arena.width / 6) * 2;
  float y1 = (arena.height / 6) * 2;

  float x2 = (arena.width / 6) * 4;
  float y2 = (arena.height / 6) * 4;

  glBegin(GL_LINE_LOOP);
    glVertex2d(x1, y1);
    glVertex2d(x2, y1);
    glVertex2d(x2, y2);
    glVertex2d(x1, y2);
  glEnd();

  float window_width = glutGet(GLUT_WINDOW_WIDTH);
  float window_height = glutGet(GLUT_WINDOW_HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, window_width, 0.0, window_height, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  std::string score = "Score: " + std::to_string((int) arena.player_points) + " | " + "Time: " + std::to_string((int) arena.time/60) + ":" + std::to_string((int) arena.time%60);
  const unsigned char msg_c [28] = "Press any key to continue";
  const unsigned char title [24] = "G A M E  O V E R  M A N";
  const char * subtitle = score.c_str();
  
  int msg_width = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (unsigned char *) msg_c) / 2.0;
  
  glColor3f(1.0, 1.0, 1.0); 
  glRasterPos2f(window_width/2 - msg_width, window_height / 2 + 50);

  for (int i = 0; i < std::strlen((char*) title); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);
  }

  glRasterPos2f(window_width/2 - msg_width, window_height / 2 - 50);

  for (int i = 0; i < std::strlen((char*) subtitle); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, subtitle[i]);
  }

  glRasterPos2f(window_width/2 - msg_width, window_height / 2);

  for (int i = 0; i < std::strlen((char*) msg_c); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, msg_c[i]);
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}

void  GraphicsRenderer_t::hud(arena_t arena) {
  
  float window_width = glutGet(GLUT_WINDOW_WIDTH);
  float window_height = glutGet(GLUT_WINDOW_HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, window_width, 0.0, window_height, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  std::string score = "Score: " + std::to_string((int) arena.player_points) + " - Next Wave: " + std::to_string((int) arena.next_wave % 60);
  std::string time = "Time: " + std::to_string((int) arena.time / 60) + ":" + std::to_string((int) arena.time % 60);

  glColor3f(1.0, 3.0, 0.0); 
  glRasterPos2f(25, window_height - 40);

  for (int i = 0; i < score.length(); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score[i]);
  }

  glRasterPos2f(window_width - 110, window_height - 40);

  for (int i = 0; i < time.length(); ++i) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, time[i]);
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}

void GraphicsRenderer_t::blackhole(BlackHole hole) {
  glPushMatrix();

  glTranslatef(hole.x, hole.y, 0.0f);
  
  for (int i = 0; i < hole.rings; ++i) {
    glRotatef(hole.ring_rotations[i], 0, 0, 1);
    glColor3f(hole.ring_colours[i][0], hole.ring_colours[i][1], hole.ring_colours[i][2]); 
    glBegin(GL_LINE_LOOP);

    for (int j = 0; j < hole.ring_points; ++j) {
      float theta = (360 / hole.ring_points) * j;
      float radian = util.deg_to_rad(theta);
      float x = cosf(radian) * hole.ring_positions[i];
      float y = sinf(radian) * hole.ring_positions[i];
      glVertex2d(x, y);
    }
    
    glEnd();
  }

  glColor3f(1.0, 1.0, 1.0); 
  glPopMatrix();
}
